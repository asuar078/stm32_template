/*
 * Semaphore.cpp
 *
 *  Created on: Oct 17, 2018
 *      Author: ArianSuarez
 */

#include <Semaphore.hpp>

namespace freertos {

  bool Semaphore::take(TickType_t xBlockTime) {
    BaseType_t success;

    success = xSemaphoreTake(handle, xBlockTime);

    return success == pdTRUE ? true : false;
  }

  bool Semaphore::takeFromISR(BaseType_t *pxHigherPriorityTaskWoken) {
    BaseType_t success;

    success = xSemaphoreTakeFromISR(handle, pxHigherPriorityTaskWoken);

    return success == pdTRUE ? true : false;
  }

  bool Semaphore::give() {
    BaseType_t success;

    success = xSemaphoreGive(handle);

    return success == pdTRUE ? true : false;
  }

  bool Semaphore::giveFromISR(BaseType_t *pxHigherPriorityTaskWoken) {
    BaseType_t success;

    success = xSemaphoreGiveFromISR(handle, pxHigherPriorityTaskWoken);

    return success == pdTRUE ? true : false;
  }

  Semaphore::Semaphore() {
  }

  Semaphore::~Semaphore() {
    vSemaphoreDelete(handle);
  }

  BinarySemaphore::BinarySemaphore(bool set) {
    #if(configSUPPORT_STATIC_ALLOCATION == 1)
    handle = xSemaphoreCreateBinaryStatic(&semaBuffer);
    #else
    handle = xSemaphoreCreateBinary();
    #endif

    if (handle == NULL) {
      configASSERT(!"BinarySemaphore Constructor Failed");
    }

    if (set) {
      xSemaphoreGive(handle);
    }
  }

  CountingSemaphore::CountingSemaphore(UBaseType_t maxCount,
                                       UBaseType_t initialCount) {
    if (maxCount == 0) {
      configASSERT(!"CountingSemaphore Constructor bad maxCount");
    }

    if (initialCount > maxCount) {
      configASSERT(!"CountingSemaphore Constructor bad initialCount");
    }

    #if(configSUPPORT_STATIC_ALLOCATION == 1)
    handle = xSemaphoreCreateCountingStatic(maxCount, initialCount, &semaBuffer);
    #else
    handle = xSemaphoreCreateCounting(maxCount, initialCount);
    #endif

    if (handle == NULL) {
      configASSERT(!"CountingSemaphore Constructor Failed");
    }
  }

} /* namespace freertos */
