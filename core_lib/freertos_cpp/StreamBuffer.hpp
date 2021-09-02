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
    StreamBuffer(size_t bufferSizeBytes, size_t triggerLevelBytes);

    virtual ~StreamBuffer();

    size_t send(const void *data,
                size_t dataLengthBytes,
                TickType_t ticksToWait = portMAX_DELAY);

    size_t sendFromISR(const void *data,
                       size_t dataLengthBytes,
                       BaseType_t *const pxHigherPriorityTaskWoken);

    size_t receive(void *data,
                   size_t dataLengthBytes,
                   TickType_t ticksToWait = portMAX_DELAY);

    size_t receiveFromISR(void *data,
                          size_t dataLengthBytes,
                          BaseType_t *const pxHigherPriorityTaskWoken);

    bool isFull() const;

    bool isEmpty() const;

    bool reset();

    size_t spaceAvailable() const;

    size_t bytesAvailable() const;

    bool setTriggerLevel(size_t triggerLevelBytes);

    protected:
    StreamBufferHandle_t handle;
  };

} /* namespace freertos */

#endif /* LIB_FREERTOS_CPP_STREAMBUFFER_HPP_ */
