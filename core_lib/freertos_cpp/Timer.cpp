
#include "Timer.hpp"

using namespace freertos;

Timer::Timer(const char* const TimerName,
    TickType_t PeriodInTicks,
    bool Periodic)
{
#if(configSUPPORT_STATIC_ALLOCATION == 1)
  handle = xTimerCreateStatic(
      TimerName,
      PeriodInTicks,
      Periodic ? pdTRUE : pdFALSE,
      this,
      timerCallbackFunctionAdapter,
      &timerBuffer);
#else
  handle = xTimerCreate(
      TimerName,
      PeriodInTicks,
      Periodic ? pdTRUE : pdFALSE,
      this,
      timerCallbackFunctionAdapter);
#endif

  if (handle == NULL) {
    configASSERT(!"Timer Constructor Failed");
  }
}

Timer::Timer(TickType_t PeriodInTicks,
    bool Periodic)
{

  #if(configSUPPORT_STATIC_ALLOCATION == 1)
  handle = xTimerCreateStatic(
      "default",
      PeriodInTicks,
      Periodic ? pdTRUE : pdFALSE,
      this,
      timerCallbackFunctionAdapter,
      &timerBuffer);
#else
  handle = xTimerCreate("default",
      PeriodInTicks,
      Periodic ? pdTRUE : pdFALSE,
      this,
      timerCallbackFunctionAdapter);
#endif

  if (handle == NULL) {
    configASSERT(!"Timer Constructor Failed");
  }
}

Timer::~Timer()
{
  xTimerDelete(handle, portMAX_DELAY);
}

bool Timer::isActive()
{
  return xTimerIsTimerActive(handle) == pdFALSE ? false : true;
}

bool Timer::start(TickType_t CmdTimeout)
{
  return xTimerStart(handle, CmdTimeout) == pdFALSE ? false : true;
}

bool Timer::startFromISR(BaseType_t* pxHigherPriorityTaskWoken)
{
  return xTimerStartFromISR(handle, pxHigherPriorityTaskWoken) == pdFALSE
         ? false
         : true;
}

bool Timer::stop(TickType_t CmdTimeout)
{
  return xTimerStop(handle, CmdTimeout) == pdFALSE ? false : true;
}

bool Timer::stopFromISR(BaseType_t* pxHigherPriorityTaskWoken)
{
  return xTimerStopFromISR(handle, pxHigherPriorityTaskWoken) == pdFALSE
         ? false
         : true;
}

bool Timer::reset(TickType_t CmdTimeout)
{
  return xTimerReset(handle, CmdTimeout) == pdFALSE ? false : true;
}

bool Timer::resetFromISR(BaseType_t* pxHigherPriorityTaskWoken)
{
  return xTimerResetFromISR(handle, pxHigherPriorityTaskWoken) == pdFALSE
         ? false
         : true;
}

bool Timer::setPeriod(TickType_t NewPeriod,
    TickType_t CmdTimeout)
{
  return xTimerChangePeriod(handle, NewPeriod, CmdTimeout) == pdFALSE
         ? false
         : true;
}

bool Timer::setPeriodFromISR(TickType_t NewPeriod,
    BaseType_t* pxHigherPriorityTaskWoken)
{
  return xTimerChangePeriodFromISR(handle, NewPeriod,
             pxHigherPriorityTaskWoken) == pdFALSE
         ? false
         : true;
}

// #if (INCLUDE_xTimerGetTimerDaemonTaskHandle == 1)

// TaskHandle_t Timer::GetTimerDaemonHandle()
// {
//     return xTimerGetTimerDaemonTaskHandle();
// }
//
// #endif

void Timer::timerCallbackFunctionAdapter(TimerHandle_t xTimer)
{
  Timer* timer = static_cast<Timer*>(pvTimerGetTimerID(xTimer));
  timer->run();
}

