#ifndef LIB_FREERTOS_CPP_TIMER_HPP_
#define LIB_FREERTOS_CPP_TIMER_HPP_

#include "FreeRTOS.h"
#include "timers.h"

namespace freertos {

/**
 *  Wrapper class around FreeRTOS's implementation of a timer.
 *
 *  This is an abstract base class.
 *  To use this, you need to subclass it. All of your timers should
 *  be derived from the Timer class. Then implement the virtual Run
 *  function. This is a similar design to Java threading.
 */
  class Timer {

    /////////////////////////////////////////////////////////////////////////
    //
    //  Public API
    //
    /////////////////////////////////////////////////////////////////////////
    public:
    /**
         *  Construct a named timer.
         *  Timers are not active after they are created, you need to
         *  activate them via Start, Reset, etc.
         *
         *  @throws TimerCreateException
         *  @param TimerName Name of the timer for debug.
         *  @param PeriodInTicks When does the timer expire and run your Run()
         *         method.
         *  @param Periodic true if the timer expires every PeriodInTicks.
         *         false if this is a one shot timer.
         */
    Timer(const char *const TimerName,
          TickType_t PeriodInTicks,
          bool Periodic = true);

    /**
         *  Construct an unnamed timer.
         *  Timers are not active after they are created, you need to
         *  activate them via Start, Reset, etc.
         *
         *  @throws TimerCreateException
         *  @param PeriodInTicks When does the timer expire and run your Run()
         *         method.
         *  @param Periodic true if the timer expires every PeriodInTicks.
         *         false if this is a one shot timer.
         */
    Timer(TickType_t PeriodInTicks,
          bool Periodic = true);

    /**
         *  Destructor
         */
    virtual ~Timer();

    /**
         *  Is the timer currently active?
         *
         *  @return true if the timer is active, false otherwise.
         */
    bool isActive();

    /**
         *  Start a timer. This changes the state to active.
         *
         *  @param CmdTimeout How long to wait to send this command to the
         *         timer code.
         *  @returns true if this command will be sent to the timer code,
         *           false if it will not (i.e. timeout).
         */
    bool start(TickType_t CmdTimeout = portMAX_DELAY);

    /**
         *  Start a timer from ISR context. This changes the state to active.
         *
         *  @param pxHigherPriorityTaskWoken Did this operation result in a
         *         rescheduling event.
         *  @returns true if this command will be sent to the timer code,
         *           false if it will not (i.e. timeout).
         */
    bool startFromISR(BaseType_t *pxHigherPriorityTaskWoken);

    /**
         *  Stop a timer. This changes the state to inactive.
         *
         *  @param CmdTimeout How long to wait to send this command to the
         *         timer code.
         *  @returns true if this command will be sent to the timer code,
         *           false if it will not (i.e. timeout).
         */
    bool stop(TickType_t CmdTimeout = portMAX_DELAY);

    /**
         *  Stop a timer from ISR context. This changes the state to inactive.
         *
         *  @param pxHigherPriorityTaskWoken Did this operation result in a
         *         rescheduling event.
         *  @returns true if this command will be sent to the timer code,
         *           false if it will not (i.e. timeout).
         */
    bool stopFromISR(BaseType_t *pxHigherPriorityTaskWoken);

    /**
         *  Reset a timer. This changes the state to active.
         *
         *  @param CmdTimeout How long to wait to send this command to the
         *         timer code.
         *  @returns true if this command will be sent to the timer code,
         *           false if it will not (i.e. timeout).
         */
    bool reset(TickType_t CmdTimeout = portMAX_DELAY);

    /**
         *  Reset a timer from ISR context. This changes the state to active.
         *
         *  @param pxHigherPriorityTaskWoken Did this operation result in a
         *         rescheduling event.
         *  @returns true if this command will be sent to the timer code,
         *           false if it will not (i.e. timeout).
         */
    bool resetFromISR(BaseType_t *pxHigherPriorityTaskWoken);

    /**
         *  Change a timer's period.
         *
         *  @param NewPeriod The period in ticks.
         *  @param CmdTimeout How long to wait to send this command to the
         *         timer code.
         *  @returns true if this command will be sent to the timer code,
         *           false if it will not (i.e. timeout).
         */
    bool setPeriod(TickType_t NewPeriod,
                   TickType_t CmdTimeout = portMAX_DELAY);

    /**
         *  Change a timer's period from ISR context.
         *
         *  @param NewPeriod The period in ticks.
         *  @param pxHigherPriorityTaskWoken Did this operation result in a
         *         rescheduling event.
         *  @returns true if this command will be sent to the timer code,
         *           false if it will not (i.e. timeout).
         */
    bool setPeriodFromISR(TickType_t NewPeriod,
                          BaseType_t *pxHigherPriorityTaskWoken);

// #if (INCLUDE_xTimerGetTimerDaemonTaskHandle == 1)
//     /**
//          *  If you need it, obtain the task handle of the FreeRTOS
//          *  task that is running the timers.
//          *
//          *  @return Task handle of the FreeRTOS timer task.
//          */
//     static TaskHandle_t getTimerDaemonHandle();
// #endif

    /////////////////////////////////////////////////////////////////////////
    //
    //  Protected API
    //  Available from inside your Thread implementation.
    //  You should make sure that you are only calling these methods
    //  from within your Run() method, or that your Run() method is on the
    //  callstack.
    //
    /////////////////////////////////////////////////////////////////////////
    protected:
    /**
         *  Implementation of your actual timer code.
         *  You must override this function.
         */
    virtual void run() = 0;

    /////////////////////////////////////////////////////////////////////////
    //
    //  Private API
    //  The internals of this wrapper class.
    //
    /////////////////////////////////////////////////////////////////////////
    private:
    /**
         *  Reference to the underlying timer handle.
         */
    TimerHandle_t handle;

    /**
         *  Adapter function that allows you to write a class
         *  specific run() function that interfaces with FreeRTOS.
         *  Look at the implementation of the constructors and this
         *  code to see how the interface between C and C++ is performed.
         */
    static void timerCallbackFunctionAdapter(TimerHandle_t xTimer);
  };

} // namespace freertos

#endif
