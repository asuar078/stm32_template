/*
 * Mutex.cpp
 *
 *  Created on: Oct 17, 2018
 *      Author: ArianSuarez
 */

#include <Mutex.hpp>

namespace freertos {

  Mutex::Mutex() {
    #if(configSUPPORT_STATIC_ALLOCATION == 1)
    handle = xSemaphoreCreateMutexStatic(&mutexBuffer);
    #else
    handle = xSemaphoreCreateMutex();
    #endif

    if (handle == nullptr) {
      configASSERT(!"Mutex Constructor Failed");
    }
  }

  Mutex::~Mutex() {
    vSemaphoreDelete(handle);
  }

  bool Mutex::lock(TickType_t Timeout) {
    BaseType_t success = xSemaphoreTake(handle, Timeout);
    return success == pdTRUE;
  }

  bool Mutex::unlock() {
    BaseType_t success = xSemaphoreGive(handle);
    return success == pdTRUE;
  }

#if (configUSE_RECURSIVE_MUTEXES == 1)

  MutexRecursive::MutexRecursive() {
    #if(configSUPPORT_STATIC_ALLOCATION == 1)
    handle = xSemaphoreCreateRecursiveMutexStatic(&mutexBuffer);
    #else
    handle = xSemaphoreCreateRecursiveMutex();
    #endif


      if (handle == NULL) {
          configASSERT(!"Mutex Constructor Failed");
      }
  }

  bool MutexRecursive::lock(TickType_t Timeout) {
      BaseType_t success = xSemaphoreTakeRecursive(handle, Timeout);
      return success == pdTRUE;
  }

  bool MutexRecursive::unlock() {
      BaseType_t success = xSemaphoreGiveRecursive(handle);
      return success == pdTRUE;
  }

#endif

} /* namespace freertos */
