#include "Queue.hpp"

namespace freertos {

  #if(configSUPPORT_STATIC_ALLOCATION == 1)

  Queue::Queue(UBaseType_t maxItems, UBaseType_t itemSize, uint8_t* storageBuffer)
  {
    handle = xQueueCreateStatic(maxItems, itemSize, storageBuffer, &queueBuffer);

    if (handle == nullptr) {
      configASSERT(!"Queue Constructor Failed");
    }
  }

  #else
  Queue::Queue(UBaseType_t maxItems, UBaseType_t itemSize) {
    handle = xQueueCreate(maxItems, itemSize);

    if (handle == nullptr) {
      configASSERT(!"Queue Constructor Failed");
    }
  }
  #endif

  Queue::~Queue()
  {
    vQueueDelete(handle);
  }

  bool Queue::enqueue(void* item, TickType_t Timeout)
  {
    BaseType_t success;

    success = xQueueSendToBack(handle, item, Timeout);

    return success == pdTRUE ? true : false;
  }

  bool Queue::dequeue(void* item, TickType_t Timeout)
  {
    BaseType_t success;

    success = xQueueReceive(handle, item, Timeout);

    return success == pdTRUE ? true : false;
  }

  bool Queue::peek(void* item, TickType_t Timeout)
  {
    BaseType_t success;

    success = xQueuePeek(handle, item, Timeout);

    return success == pdTRUE ? true : false;
  }

  bool Queue::enqueueFromISR(void* item, BaseType_t* pxHigherPriorityTaskWoken)
  {
    BaseType_t success;

    success = xQueueSendToBackFromISR(handle, item, pxHigherPriorityTaskWoken);

    return success == pdTRUE ? true : false;
  }

  bool Queue::dequeueFromISR(void* item, BaseType_t* pxHigherPriorityTaskWoken)
  {
    BaseType_t success;

    success = xQueueReceiveFromISR(handle, item, pxHigherPriorityTaskWoken);

    return success == pdTRUE ? true : false;
  }

  bool Queue::peekFromISR(void* item)
  {
    BaseType_t success;

    success = xQueuePeekFromISR(handle, item);

    return success == pdTRUE ? true : false;
  }

  bool Queue::isEmpty()
  {
    UBaseType_t cnt = uxQueueMessagesWaiting(handle);

    return cnt == 0 ? true : false;
  }

  bool Queue::isFull()
  {
    UBaseType_t cnt = uxQueueSpacesAvailable(handle);

    return cnt == 0 ? true : false;
  }

  void Queue::flush()
  {
    xQueueReset(handle);
  }

  UBaseType_t Queue::numItems()
  {
    return uxQueueMessagesWaiting(handle);
  }

  UBaseType_t Queue::numSpacesLeft()
  {
    return uxQueueSpacesAvailable(handle);
  }

//  #if(configSUPPORT_STATIC_ALLOCATION == 1)
//
//  Deque::Deque(UBaseType_t maxItems, UBaseType_t itemSize, uint8_t* storageBuffer)
//      :Queue(maxItems, itemSize, storageBuffer)
//  {
//  }
//
//  #else
//  Deque::Deque(UBaseType_t maxItems, UBaseType_t itemSize)
//      :Queue(maxItems, itemSize)
//  {
//  }
//  #endif

  bool Deque::enqueueToFront(void* item, TickType_t Timeout)
  {
    BaseType_t success;

    success = xQueueSendToFront(handle, item, Timeout);

    return success == pdTRUE ? true : false;
  }

  bool Deque::enqueueToFrontFromISR(void* item, BaseType_t* pxHigherPriorityTaskWoken)
  {
    BaseType_t success;

    success = xQueueSendToFrontFromISR(handle, item, pxHigherPriorityTaskWoken);

    return success == pdTRUE ? true : false;
  }

  #if(configSUPPORT_STATIC_ALLOCATION == 1)

  BinaryQueue::BinaryQueue(UBaseType_t itemSize, uint8_t* storageBuffer)
      :Queue(1, itemSize, storageBuffer)
  {
  }

  #else

  BinaryQueue::BinaryQueue(UBaseType_t itemSize)
      :Queue(1, itemSize)
  {
  }

  #endif

  bool BinaryQueue::enqueue(void* item, TickType_t Timeout)
  {
    (void) xQueueOverwrite(handle, item);
    return true;
  }

  bool BinaryQueue::enqueueFromISR(void* item, BaseType_t* pxHigherPriorityTaskWoken)
  {
    (void) xQueueOverwriteFromISR(handle, item, pxHigherPriorityTaskWoken);
    return true;
  }


} // namespace freertos
