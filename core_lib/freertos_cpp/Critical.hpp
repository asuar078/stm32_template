/*
 * Critical.hpp
 *
 *  Created on: Oct 17, 2018
 *      Author: ArianSuarez
 */

#ifndef LIB_FREERTOS_CPP_CRITICAL_HPP_
#define LIB_FREERTOS_CPP_CRITICAL_HPP_

#include "FreeRTOS.h"
#include "task.h"

namespace freertos {

  /**
   *  Wrapper class around various critical section type
   *  synchronization mechanisms within FreeRTOS.
   */
  class CriticalSection {

    /////////////////////////////////////////////////////////////////////////
    //
    //  Public API
    //  Available from anywhere.
    //
    /////////////////////////////////////////////////////////////////////////
    public:
    /**
     *  Disable context switches as well as maskable interrupts.
     */
    static inline void enter() {
      taskENTER_CRITICAL();
    }

    /**
     *  Re-enable context switches.
     */
    static inline void exit() {
      taskEXIT_CRITICAL();
    }

    /**
     *  Disable context switches as well as maskable interrupts
     *  from an interrupt context.
     *
     *  @return Opaque representation of interrupt mask state.
     *  This must be passed back to the corresponding call to
     *  ExitFromISR().
     *
     *  @note See the following for further details:
     *  http://www.freertos.org/taskENTER_CRITICAL_FROM_ISR_taskEXIT_CRITICAL_FROM_ISR.html
     */
    static inline BaseType_t enterFromISR() {
      return taskENTER_CRITICAL_FROM_ISR();
    }

    /**
     *  Re-enable context switches from an interrupt context.
     *
     *  @param savedInterruptStatus This should be the value you
     *  received from calling EnterFromISR().
     *
     *  @note See the following for further details:
     *  http://www.freertos.org/taskENTER_CRITICAL_FROM_ISR_taskEXIT_CRITICAL_FROM_ISR.html
     */
    static inline void exitFromISR(BaseType_t savedInterruptStatus) {
      taskEXIT_CRITICAL_FROM_ISR(savedInterruptStatus);
    }

    /**
     *  Disable all maskable interrupts.
     */
    static inline void disableInterrupts() {
      taskDISABLE_INTERRUPTS();
    }

    /**
     *  Enable all maskable interrupts.
     */
    static inline void enableInterrupts() {
      taskENABLE_INTERRUPTS();
    }

    /**
     *  Suspend the scheduler without disabling interrupts.
     */
    static inline void suspendScheduler() {
      vTaskSuspendAll();
    }

    /**
     *  Re-enable the scheduler.
     */
    static inline void resumeScheduler() {
      xTaskResumeAll();
    }
  };

}

#endif /* LIB_FREERTOS_CPP_CRITICAL_HPP_ */
