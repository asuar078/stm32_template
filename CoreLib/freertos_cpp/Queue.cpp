#include "Queue.hpp"

namespace freertos {

  Queue::Queue(UBaseType_t maxItems, UBaseType_t itemSize) {
    handle = xQueueCreate(maxItems, itemSize);

    if (handle == NULL) {
      configASSERT(!"Queue Constructor Failed");
    }
  }

  Queue::~Queue() {
    vQueueDelete(handle);
  }

  bool Queue::enqueue(void *item, TickType_t Timeout) {
    BaseType_t success;

    success = xQueueSendToBack(handle, item, Timeout);

    return success == pdTRUE ? true : false;
  }

  bool Queue::dequeue(void *item, TickType_t Timeout) {
    BaseType_t success;

    success = xQueueReceive(handle, item, Timeout);

    return success == pdTRUE ? true : false;
  }

  bool Queue::peek(void *item, TickType_t Timeout) {
    BaseType_t success;

    success = xQueuePeek(handle, item, Timeout);

    return success == pdTRUE ? true : false;
  }

  bool Queue::enqueueFromISR(void *item, BaseType_t *pxHigherPriorityTaskWoken) {
    BaseType_t success;

    success = xQueueSendToBackFromISR(handle, item, pxHigherPriorityTaskWoken);

    return success == pdTRUE ? true : false;
  }

  bool Queue::dequeueFromISR(void *item, BaseType_t *pxHigherPriorityTaskWoken) {
    BaseType_t success;

    success = xQueueReceiveFromISR(handle, item, pxHigherPriorityTaskWoken);

    return success == pdTRUE ? true : false;
  }

  bool Queue::peekFromISR(void *item) {
    BaseType_t success;

    success = xQueuePeekFromISR(handle, item);

    return success == pdTRUE ? true : false;
  }

  bool Queue::isEmpty() {
    UBaseType_t cnt = uxQueueMessagesWaiting(handle);

    return cnt == 0 ? true : false;
  }

  bool Queue::isFull() {
    UBaseType_t cnt = uxQueueSpacesAvailable(handle);

    return cnt == 0 ? true : false;
  }

  void Queue::flush() {
    xQueueReset(handle);
  }

  UBaseType_t Queue::numItems() {
    return uxQueueMessagesWaiting(handle);
  }

  UBaseType_t Queue::numSpacesLeft() {
    return uxQueueSpacesAvailable(handle);
  }

  Deque::Deque(UBaseType_t maxItems, UBaseType_t itemSize)
      : Queue(maxItems, itemSize) {
  }

  bool Deque::enqueueToFront(void *item, TickType_t Timeout) {
    BaseType_t success;

    success = xQueueSendToFront(handle, item, Timeout);

    return success == pdTRUE ? true : false;
  }

  bool Deque::enqueueToFrontFromISR(void *item, BaseType_t *pxHigherPriorityTaskWoken) {
    BaseType_t success;

    success = xQueueSendToFrontFromISR(handle, item, pxHigherPriorityTaskWoken);

    return success == pdTRUE ? true : false;
  }

  BinaryQueue::BinaryQueue(UBaseType_t itemSize)
      : Queue(1, itemSize) {
  }

  bool BinaryQueue::enqueue(void *item, TickType_t Timeout) {
    (void) xQueueOverwrite(handle, item);
    return true;
  }

  bool BinaryQueue::enqueueFromISR(void *item, BaseType_t *pxHigherPriorityTaskWoken) {
    (void) xQueueOverwriteFromISR(handle, item, pxHigherPriorityTaskWoken);
    return true;
  }

} // namespace freertos
