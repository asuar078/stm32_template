/*
 * EventGroup.cpp
 *
 *  Created on: Oct 17, 2018
 *      Author: ArianSuarez
 */

#include <EventGroup.hpp>

namespace freertos {

  EventGroup::EventGroup()
  {
    #if(configSUPPORT_STATIC_ALLOCATION == 1)
    handle = xEventGroupCreateStatic(&eventGroupBuffer);
    #else
    handle = xEventGroupCreate();
    #endif

    if (handle == nullptr) {
      configASSERT(!"EventGroup Constructor Failed");
    }

  }

  EventGroup::~EventGroup()
  {
    vEventGroupDelete(handle);
  }

  EventBits_t EventGroup::Sync(const EventBits_t uxBitsToSet,
      const EventBits_t uxBitsToWaitFor,
      TickType_t xTicksToWait)
  {

    return xEventGroupSync(handle, uxBitsToSet, uxBitsToWaitFor, xTicksToWait);

  }

  EventBits_t EventGroup::WaitBits(const EventBits_t uxBitsToWaitFor,
      bool xClearOnExit, bool xWaitForAllBits,
      TickType_t xTicksToWait)
  {

    return xEventGroupWaitBits(handle, uxBitsToWaitFor,
        xClearOnExit ? pdTRUE : pdFALSE,
        xWaitForAllBits ? pdTRUE : pdFALSE, xTicksToWait);
  }

  EventBits_t EventGroup::ClearBits(const EventBits_t uxBitsToClear)
  {
    return xEventGroupClearBits(handle, uxBitsToClear);
  }

//  BaseType_t EventGroup::ClearBitsFromISR(const EventBits_t uxBitsToClear) {
//    return xEventGroupClearBitsFromISR(handle, uxBitsToClear);
//  }

  EventBits_t EventGroup::GetBits()
  {
    return xEventGroupGetBits(handle);
  }

  EventBits_t EventGroup::GetBitsFromISR()
  {
    return xEventGroupGetBitsFromISR(handle);
  }

  EventBits_t EventGroup::SetBits(const EventBits_t uxBitsToSet)
  {
    return xEventGroupSetBits(handle, uxBitsToSet);
  }

#if ((configUSE_TRACE_FACILITY == 1) && (INCLUDE_xTimerPendFunctionCall == 1) && (configUSE_TIMERS == 1))

  BaseType_t EventGroup::SetBitsFromISR(const EventBits_t uxBitsToSet, BaseType_t* pxHigherPriorityTaskWoken)
  {
    return xEventGroupSetBitsFromISR(handle, uxBitsToSet, pxHigherPriorityTaskWoken);
  }

#endif
}
/* namespace freertos */
