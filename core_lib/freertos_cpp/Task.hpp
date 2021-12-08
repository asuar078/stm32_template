/*
 * Task.h
 *
 *  Created on: Mar 4, 2017
 *      Author: kolban
 */

#ifndef LIB_FREERTOS_CPP_TASK_HPP_
#define LIB_FREERTOS_CPP_TASK_HPP_

#include <FreeRTOS.h>
#include <task.h>

#define loop while(true)

namespace freertos {

  class Task {
    public:
      explicit Task(const char* taskName, uint16_t stackSize = 128, uint8_t priority = 5);

      virtual ~Task();

      void setStackSize(uint16_t stackSize);

      uint16_t getStackSize();

      void setName(const char* name);

      const char* getName();

      /**
      *  Accessor to get the thread's backing task handle.
      *  There is no setter, on purpose.
      *
      *  @return FreeRTOS task handle.
      */
      inline TaskHandle_t getHandle()
      {
        return m_handle;
      }

      /**
         *  Starts a thread.
         *
         *  This is the API call that actually starts the thread running.
         *  It creates a backing FreeRTOS task. By separating object creation
         *  from starting the Thread, it solves the pure virtual fuction call
         *  failure case. If we attempt to automatically call xTaskCreate
         *  from the base class constructor, in certain conditions the task
         *  starts to run "before" the derived class is constructed! So we
         *  don't do that anymore.
         *
         *  This may be called from your ctor once you have completed
         *  your objects construction (so as the last step).
         *
         *  This should only be called once ever!
         */
      bool start(void* taskData);

      void stop();

      /**
     * @brief Suspend the task for the specified milliseconds.
     *
     * @param [in] ms The delay time in milliseconds.
     * @return N/A.
     */
      static inline void delay_ms(uint32_t ms)
      {
        ::vTaskDelay(ms / portTICK_PERIOD_MS);
      }

      /**
       *  Yield the scheduler.
       */
      static inline void yield()
      {
        taskYIELD();
      }

      static inline void yieldFromISR(BaseType_t pxHigherPriorityTaskWoken)
      {
        portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
      }

      /**
       *  Start the scheduler.
       *
       *  @note You need to use this call. Do NOT directly call
       *  vTaskStartScheduler while using this library.
       */
      static inline void startScheduler()
      {
        vTaskStartScheduler();
      }

      /**
           *  End the scheduler.
           *
           *  @note Please see the FreeRTOS documentation regarding constraints
           *  with the implementation of this.
           *
           *  @note You need to use this call. Do NOT directly call
           *  vTaskEndScheduler while using this library.
           */
      static inline void endScheduler()
      {
        vTaskEndScheduler();
      }

#if (INCLUDE_vTaskSuspend == 1)

      /**
           *  Suspend this thread.
           *
           *  @note While a Thread can Suspend() itself, it cannot Resume()
           *  itself, becauseit's suspended.
           */
      inline void suspend()
      {
        vTaskSuspend(getHandle());
      }

      /**
           *  Resume a specific thread.
           */
      inline void resume()
      {
        vTaskResume(getHandle());
      }

#endif

#if (INCLUDE_xTaskResumeFromISR == 1)

      /**
           *  Resume a specific thread from ISR context.
           */
      inline void resumeFromISR()
      {
        xTaskResumeFromISR(getHandle());
      }

#endif

#if (INCLUDE_uxTaskPriorityGet == 1)

      /**
           *  Get the priority of this Thread.
           *
           *  @return Priority at the time this was called.
           */
      inline UBaseType_t getPriority()
      {
        return (uxTaskPriorityGet(getHandle()));
      }

      /**
           *  Get the priority of this Thread from ISR context.
           *
           *  @return Priority at the time this was called.
           */
      inline UBaseType_t getPriorityFromISR()
      {
        return (uxTaskPriorityGetFromISR(getHandle()));
      }

#endif

#if (INCLUDE_vTaskPrioritySet == 1)

      /**
           *  Set the priority of this thread.
           *
           *  @param NewPriority The thread's new priority.
           */
      inline void setPriority(UBaseType_t newPriority)
      {
        m_priority = newPriority;
        vTaskPrioritySet(getHandle(), newPriority);
      }

#endif

    protected:
      const char* m_taskName;
      uint16_t m_stackSize;
      uint8_t m_priority;
      void* m_taskData;
      TaskHandle_t m_handle;

      /**
       *  Implementation of your actual thread code.
       *  You must override this function.
       *  @note If INCLUDE_vTaskDelete is defined, then you may return from
       *  your Run method. This will cause the task to be deleted from
       *  FreeRTOS, however you are still responsible to delete the
       *  task object. If this is not defined, then retuning from your Run()
       *  method will result in an assert.
       */
      virtual void run() = 0;

      /**
       *  Called on exit from your Run() routine.
       *
       *  It is optional whether you implement this or not.
       *
       *  If you allow your Thread to exit its Run method,
       *  implementing a Cleanup method allows you to call
       *  your Thread's destructor. If you decide to call delete
       *  in your Cleanup function, be aware that additional
       *  derived classes shouldn't call delete.
       */
      virtual void cleanup();

#if (INCLUDE_vTaskDelayUntil == 1)

      /**
       *  Flag denoting if we've setup delay until yet.
       */
      bool delayUntilInitialized{};

      /**
       *  Book keeping value for delay until.
       */
      TickType_t delayUntilPreviousWakeTime{};

      /**
       *  Delay this thread for Period ticks, taking into account
       *  the execution time of the thread.
       *
       *  This FreeRTOS permutation of delay can be used by periodic
       *  tasks to ensure a constant execution frequency.
       *
       *  @param Period How long to delay the thread.
       */
      void delayUntil(const TickType_t Period);

      /**
       *  If you need to adjust or reset the period of the
       *  DelayUntil method.
       */
      void resetDelayUntil();

#endif

    private:
      /**
       *  Adapter function that allows you to write a class
       *  specific Run() function that interfaces with FreeRTOS.
       *  Look at the implementation of the constructors and this
       *  code to see how the interface between C and C++ is performed.
       */
      static void taskFunctionAdapter(void* pvParameters);

      /**
       *  Flag whether or not the Thread was started.
       */
      bool taskStarted;
  };
} // namespace freertos

#endif /* COMPONENTS_CPP_UTILS_TASK_H_ */
