/*
 * StreamBuffer.hpp
 *
 *  Created on: Sep 11, 2019
 *      Author: asuarez
 */

#ifndef LIB_FREERTOS_CPP_STREAMBUFFER_HPP_
#define LIB_FREERTOS_CPP_STREAMBUFFER_HPP_

#include "FreeRTOS.h"
#include "stream_buffer.h"

namespace freertos {

  class StreamBuffer {
    public:

      #if(configSUPPORT_STATIC_ALLOCATION == 1)

      StreamBuffer(size_t bufferSizeBytes, size_t triggerLevelBytes, uint8_t* storageBuffer);

      #else

      StreamBuffer(size_t bufferSizeBytes, size_t triggerLevelBytes);

      #endif


      virtual ~StreamBuffer();

      size_t send(const void* data,
          size_t dataLengthBytes,
          TickType_t ticksToWait = portMAX_DELAY);

      size_t sendFromISR(const void* data,
          size_t dataLengthBytes,
          BaseType_t* const pxHigherPriorityTaskWoken);

      size_t receive(void* data,
          size_t dataLengthBytes,
          TickType_t ticksToWait = portMAX_DELAY);

      size_t receiveFromISR(void* data,
          size_t dataLengthBytes,
          BaseType_t* const pxHigherPriorityTaskWoken);

      bool isFull() const;

      bool isEmpty() const;

      bool reset();

      [[nodiscard]] size_t spaceAvailable() const;

      [[nodiscard]] size_t bytesAvailable() const;

      bool setTriggerLevel(size_t triggerLevelBytes);

    protected:
      StreamBufferHandle_t handle;

      #if(configSUPPORT_STATIC_ALLOCATION == 1)
      StaticStreamBuffer_t staticStreamBuffer{};
      #endif
  };

} /* namespace freertos */

#endif /* LIB_FREERTOS_CPP_STREAMBUFFER_HPP_ */
