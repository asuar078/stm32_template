/*
 * Mutex.cpp
 *
 *  Created on: Oct 17, 2018
 *      Author: ArianSuarez
 */

#include <Mutex.hpp>

namespace freertos {

  Mutex::Mutex() {
    handle = xSemaphoreCreateMutex();

    if (handle == NULL) {
      configASSERT(!"Mutex Constructor Failed");
    }
  }

  Mutex::~Mutex() {
    vSemaphoreDelete(handle);
  }

  bool Mutex::lock(TickType_t Timeout) {
    BaseType_t success = xSemaphoreTake(handle, Timeout);
    return success == pdTRUE ? true : false;
  }

  bool Mutex::unlock() {
    BaseType_t success = xSemaphoreGive(handle);
    return success == pdTRUE ? true : false;
  }

#if (configUSE_RECURSIVE_MUTEXES == 1)

  MutexRecursive::MutexRecursive() {
      handle = xSemaphoreCreateRecursiveMutex();

      if (handle == NULL) {
          configASSERT(!"Mutex Constructor Failed");
      }
  }

  bool MutexRecursive::lock(TickType_t Timeout) {
      BaseType_t success = xSemaphoreTakeRecursive(handle, Timeout);
      return success == pdTRUE ? true : false;
  }

  bool MutexRecursive::unlock() {
      BaseType_t success = xSemaphoreGiveRecursive(handle);
      return success == pdTRUE ? true : false;
  }

#endif

} /* namespace freertos */
