/*
 * StreamBuffer.cpp
 *
 *  Created on: Sep 11, 2019
 *      Author: asuarez
 */

#include <StreamBuffer.hpp>

namespace freertos {

  StreamBuffer::StreamBuffer(size_t bufferSizeBytes, size_t triggerLevelBytes) {
    handle = xStreamBufferCreate(bufferSizeBytes, triggerLevelBytes);

    if (handle == NULL) {
      configASSERT(!"Stream Buffer Constructor Failed");
    }
  }

  StreamBuffer::~StreamBuffer() {
    vStreamBufferDelete(handle);
  }

  size_t StreamBuffer::send(const void *data, size_t dataLengthBytes,
                            TickType_t ticksToWait) {
    return xStreamBufferSend(handle, data, dataLengthBytes, ticksToWait);
  }

  size_t StreamBuffer::sendFromISR(
      const void *data, size_t dataLengthBytes,
      BaseType_t *const pxHigherPriorityTaskWoken) {
    return xStreamBufferSendFromISR(handle, data, dataLengthBytes,
                                    pxHigherPriorityTaskWoken);

  }

  size_t StreamBuffer::receive(void *data, size_t dataLengthBytes,
                               TickType_t ticksToWait) {
    return xStreamBufferReceive(handle, data, dataLengthBytes, ticksToWait);
  }

  size_t StreamBuffer::receiveFromISR(
      void *data, size_t dataLengthBytes,
      BaseType_t *const pxHigherPriorityTaskWoken) {
    return xStreamBufferReceiveFromISR(handle, data, dataLengthBytes,
                                       pxHigherPriorityTaskWoken);
  }

  bool StreamBuffer::isFull() const {
    BaseType_t success;

    success = xStreamBufferIsFull(handle);

    return success == pdTRUE ? true : false;
  }

  bool StreamBuffer::isEmpty() const {
    BaseType_t success;

    success = xStreamBufferIsEmpty(handle);

    return success == pdTRUE ? true : false;
  }

  bool StreamBuffer::reset() {
    BaseType_t success;

    success = xStreamBufferReset(handle);

    return success == pdTRUE ? true : false;
  }

  size_t StreamBuffer::spaceAvailable() const {
    return xStreamBufferSpacesAvailable(handle);
  }

  size_t StreamBuffer::bytesAvailable() const {
    return xStreamBufferBytesAvailable(handle);
  }

  bool StreamBuffer::setTriggerLevel(size_t triggerLevelBytes) {
    BaseType_t success;

    success = xStreamBufferSetTriggerLevel(handle, triggerLevelBytes);

    return success == pdTRUE ? true : false;
  }

} /* namespace freertos */
