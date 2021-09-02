/*
** EPITECH PROJECT, 2019
** FreeRTOS Queue cpp
** File description:
** FreeRTOS Queue wrapper in c++
*/

#ifndef LIB_FREERTOS_CPP_QUEUE_HPP
#define LIB_FREERTOS_CPP_QUEUE_HPP

#include "FreeRTOS.h"
#include "queue.h"

namespace freertos {

/**
 *  Queue class wrapper for FreeRTOS queues. This class provides enqueue
 *  and dequeue operations.
 *
 *  @note It is expected that an application will instantiate this class or
 *        one of the derived classes and use that. It is not expected that
 *        a user or application will derive from these classes.
 */
  class Queue {

    /////////////////////////////////////////////////////////////////////////
    //
    //  Public API
    //
    /////////////////////////////////////////////////////////////////////////
    public:
    /**
        *  Our constructor.
        *
        *  @throws QueueCreateException
        *  @param maxItems Maximum number of items this queue can hold.
        *  @param itemSize Size of an item in a queue.
        *  @note FreeRTOS queues use a memcpy / fixed size scheme for queues.
        */
    Queue(UBaseType_t maxItems, UBaseType_t itemSize);

    /**
        *  Our destructor.
        */
    virtual ~Queue();

    /**
        *  Add an item to the back of the queue.
        *
        *  @param item The item you are adding.
        *  @param Timeout How long to wait to add the item to the queue if
        *         the queue is currently full.
        *  @return true if the item was added, false if it was not.
        */
    virtual bool enqueue(void *item, TickType_t Timeout = portMAX_DELAY);

    /**
        *  Remove an item from the front of the queue.
        *
        *  @param item Where the item you are removing will be returned to.
        *  @param Timeout How long to wait to remove an item if the queue
        *         is currently empty.
        *  @return true if an item was removed, false if no item was removed.
        */
    bool dequeue(void *item, TickType_t Timeout = portMAX_DELAY);

    /**
        *  Make a copy of an item from the front of the queue. This will
        *  not remove it from the head of the queue.
        *
        *  @param item Where the item you are removing will be returned to.
        *  @param Timeout How long to wait to remove an item if the queue
        *         is currently empty.
        *  @return true if an item was copied, false if no item was copied.
        */
    bool peek(void *item, TickType_t Timeout = portMAX_DELAY);

    /**
        *  Add an item to the back of the queue in ISR context.
        *
        *  @param item The item you are adding.
        *  @param pxHigherPriorityTaskWoken Did this operation result in a
        *         rescheduling event.
        *  @return true if the item was added, false if it was not.
        */
    virtual bool enqueueFromISR(void *item, BaseType_t *pxHigherPriorityTaskWoken);

    /**
        *  Remove an item from the front of the queue in ISR context.
        *
        *  @param item Where the item you are removing will be returned to.
        *  @param pxHigherPriorityTaskWoken Did this operation result in a
        *         rescheduling event.
        *  @return true if an item was removed, false if no item was removed.
        */
    bool dequeueFromISR(void *item, BaseType_t *pxHigherPriorityTaskWoken);

    /**
        *  Make a copy of an item from the front of the queue. This will
        *  not remove it from the head of the queue.
        *
        *  @param item Where the item you are removing will be returned to.
        *  @return true if an item was copied, false if no item was copied.
        */
    bool peekFromISR(void *item);

    /**
        *  Is the queue empty?
        *  @return true if the queue was empty when this was called, false if
        *  the queue was not empty.
        */
    bool isEmpty();

    /**
        *  Is the queue full?
        *  @return true if the queue was full when this was called, false if
        *  the queue was not full.
        */
    bool isFull();

    /**
        *  Remove all objects from the queue.
        */
    void flush();

    /**
        *  How many items are currently in the queue.
        *  @return the number of items in the queue.
        */
    UBaseType_t numItems();

    /**
        *  How many empty spaves are currently left in the queue.
        *  @return the number of remaining spaces.
        */
    UBaseType_t numSpacesLeft();

    /////////////////////////////////////////////////////////////////////////
    //
    //  Protected API
    //  Not intended for use by application code.
    //
    /////////////////////////////////////////////////////////////////////////
    protected:
    /**
        *  FreeRTOS queue handle.
        */
    QueueHandle_t handle;
  };

/**
 *  Enhanced queue class that implements a double ended queue (a "deque"),
 *  almost. Unlike the traditional CommSci version, there is no way to
 *  dequeue from the back. Practically, this most likely isn't a big deal.
 *
 *  @note It is expected that an application will instantiate this class or
 *        one of the derived classes and use that. It is not expected that
 *        a user or application will derive from these classes.
 */
  class Deque : public Queue {

    /////////////////////////////////////////////////////////////////////////
    //
    //  Public API
    //
    /////////////////////////////////////////////////////////////////////////
    public:
    /**
        *  Our constructor.
        *
        *  @throws QueueCreateException
        *  @param maxItems Maximum number of items thsi queue can hold.
        *  @param itemSize Size of an item in a queue.
        *  @note FreeRTOS queues use a memcpy / fixed size scheme for queues.
        */
    Deque(UBaseType_t maxItems, UBaseType_t itemSize);

    /**
        *  Add an item to the front of the queue. This will result in
        *  the item being removed first, ahead of all of the items
        *  added by the base calss dequeue() function.
        *
        *  @param item The item you are adding.
        *  @param Timeout How long to wait to add the item to the queue if
        *         the queue is currently full.
        *  @return true if the item was added, false if it was not.
        */
    bool enqueueToFront(void *item, TickType_t Timeout = portMAX_DELAY);

    /**
        *  Add an item to the front of the queue. This will result in
        *  the item being removed first, ahead of all of the items
        *  added by the base calss dequeue() function.
        *
        *  @param item The item you are adding.
        *  @param pxHigherPriorityTaskWoken Did this operation result in a
        *         rescheduling event.
        *  @return true if the item was added, false if it was not.
        */
    bool enqueueToFrontFromISR(void *item, BaseType_t *pxHigherPriorityTaskWoken);
  };

/**
 *  Binary queue with overwrite. This queue can only hold one item.
 *  If sucessive Enqueue operations are called, that item is overwritten
 *  with whatever the last item was.
 *
 *  @note It is expected that an application will instantiate this class or
 *        one of the derived classes and use that. It is not expected that
 *        a user or application will derive from these classes.
 */
  class BinaryQueue : public Queue {

    /////////////////////////////////////////////////////////////////////////
    //
    //  Public API
    //
    /////////////////////////////////////////////////////////////////////////
    public:
    /**
        *  Our constructor.
        *
        *  @throws QueueCreateException
        *  @param itemSize Size of an item in a queue.
        *  @note FreeRTOS queues use a memcpy / fixed size scheme for queues.
        */
    explicit BinaryQueue(UBaseType_t itemSize);

    /**
         *  Add an item to the queue.
         *
         *  @param item The item you are adding.
         *  @return true always, because of overwrite.
         */
    virtual bool enqueue(void *item, TickType_t Timeout = portMAX_DELAY) override;

    /**
         *  Add an item to the queue in ISR context.
         *
         *  @param item The item you are adding.
         *  @param pxHigherPriorityTaskWoken Did this operation result in a
         *         rescheduling event.
         *  @return true always, because of overwrite.
         */
    virtual bool enqueueFromISR(void *item, BaseType_t *pxHigherPriorityTaskWoken) override;
  };

} // namespace freertos
#endif
