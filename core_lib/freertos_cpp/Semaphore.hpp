/*
 * Semaphore.hpp
 *
 *  Created on: Oct 17, 2018
 *      Author: ArianSuarez
 */

#ifndef LIB_FREERTOS_CPP_SEMAPHORE_HPP_
#define LIB_FREERTOS_CPP_SEMAPHORE_HPP_

#include "FreeRTOS.h"
#include "semphr.h"

namespace freertos {

  /**
   *
   *  Base wrapper class around FreeRTOS's implementation of semaphores.
   *
   *  It is not expected that an application will derive from this class.
   *
   *  Note that we distinguish between Semaphore, Binary Semaphores,
   *  Counting Semaphores, and Mutexes. Mutexes, while implemented as a kind
   *  of semaphore in FreeRTOS, are conceptually very different in use and
   *  behavior from semaphores. We acknowledge this difference in the class
   *  heirarchy, implementing mutextes as a completely different class heirarchy.
   */
  class Semaphore {

    /////////////////////////////////////////////////////////////////////////
    //
    //  Public API
    //
    /////////////////////////////////////////////////////////////////////////
    public:
    /**
     *  Aquire (take) a semaphore.
     *
     *  Example of blocking indefinitely:
     *      aSemaphore.Take();
     *
     *  Example of blocking for 100 ticks:
     *      aSemaphore.Take(100);
     *
     *  @param Timeout How long to wait to get the Lock until giving up.
     *  @return true if the Semaphore was acquired, false if it timed out.
     */
    bool take(TickType_t Timeout = portMAX_DELAY);

    /**
     *  Release (give) a semaphore.
     *
     *  @return true if the Semaphore was released, false if it failed.
     */
    bool give();

    /**
     *  Aquire (take) a semaphore from ISR context.
     *
     *  @param pxHigherPriorityTaskWoken Did this operation result in a
     *         rescheduling event.
     *  @return true if the Semaphore was acquired, false if it timed out.
     */
    bool takeFromISR(BaseType_t *pxHigherPriorityTaskWoken);

    /**
     *  Release (give) a semaphore from ISR context.
     *
     *  @param pxHigherPriorityTaskWoken Did this operation result in a
     *         rescheduling event.
     *  @return true if the Semaphore was released, false if it failed.
     */
    bool giveFromISR(BaseType_t *pxHigherPriorityTaskWoken);

    /**
     *  Our destructor
     */
    virtual ~Semaphore();

    /////////////////////////////////////////////////////////////////////////
    //
    //  Protected API
    //  Not intended for use by application code.
    //
    /////////////////////////////////////////////////////////////////////////
    protected:
    /**
     *  FreeRTOS semaphore handle.
     */
    SemaphoreHandle_t handle;

    /**
     *  We do not want a Semaphore ctor. This class should never be
     *  directly created, this is a base class only.
     */
    Semaphore();
  };

  /**
   *  Wrapper class for Binary Semaphores.
   */
  class BinarySemaphore : public Semaphore {

    /////////////////////////////////////////////////////////////////////////
    //
    //  Public API
    //
    /////////////////////////////////////////////////////////////////////////
    public:
    /**
     *  Constructor to create a binary semaphore.
     *
     *  @param set Is this semaphore "full" or not?
     *  @throws SemaphoreCreateException on failure.
     *  @return Instance of a BinarySemaphore.
     */
    explicit BinarySemaphore(bool set = false);
  };

  /**
   *  Wrapper class for Counting Semaphores.
   */
  class CountingSemaphore : public Semaphore {

    /////////////////////////////////////////////////////////////////////////
    //
    //  Public API
    //
    /////////////////////////////////////////////////////////////////////////
    public:
    /**
     *  Constructor to create a counting semaphore.
     *  This ctor throws a SemaphoreCreateException on failure.
     *
     *  @param maxCount Must be greater than 0.
     *  @param initialCount Must not be greater than maxCount.
     *  @throws SemaphoreCreateException on failure.
     *  @return Instance of a CountingSemaphore.
     */
    CountingSemaphore(UBaseType_t maxCount, UBaseType_t initialCount);
  };

} /* namespace freertos */

#endif /* LIB_FREERTOS_CPP_SEMAPHORE_HPP_ */
