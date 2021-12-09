/*
 * Mutex.hpp
 *
 *  Created on: Oct 17, 2018
 *      Author: ArianSuarez
 */

#ifndef LIB_FREERTOS_CPP_MUTEX_HPP_
#define LIB_FREERTOS_CPP_MUTEX_HPP_

#ifndef UNIT_TEST

#include "FreeRTOS.h"
#include "semphr.h"

#else

#include "FreertosMock.hpp"

#endif

namespace freertos {

#ifndef UNIT_TEST

  /**
   *  Standard usage Mutex.
   *  By default calls to Lock these objects block forever, but this can be
   *  changed by simply passing in a argument to the Lock() method.
   *  These objects are not recursively acquirable. Calling Lock() twice from
   *  the same Thread (i.e. task) will deadlock.
   *
   *  @note Standard mutexes use less resources than recursive mutexes. You
   *        should typically use this type of Mutex, unless you have a strong
   *        need for a MutexRecursive mutex.
   */
  class Mutex {

      /////////////////////////////////////////////////////////////////////////
      //
      //  Public API
      //
      /////////////////////////////////////////////////////////////////////////
    public:
      /**
       *  Create a standard, non-recursize Mutex.
       */
      Mutex();

      virtual ~Mutex();

      /**
       *  Lock the Mutex.
       *
       *  @param Timeout How long to wait to get the Lock until giving up.
       *  @return true if the Lock was acquired, false if it timed out.
       */
      bool lock(TickType_t Timeout = portMAX_DELAY);

      /**
       *  Unlock the Mutex.
       *
       *  @return true if the Lock was released, false if it failed. (Hint,
       *           if it fails, did you call Lock() first?)
       */
      bool unlock();

    protected:
      SemaphoreHandle_t handle;
      #if(configSUPPORT_STATIC_ALLOCATION == 1)
      StaticSemaphore_t mutexBuffer{};
      #endif
  };

#else

  class Mutex {

    /////////////////////////////////////////////////////////////////////////
    //
    //  Public API
    //
    /////////////////////////////////////////////////////////////////////////
    public:
    virtual ~Mutex();

    /**
     *  Lock the Mutex.
     *
     *  @param Timeout How long to wait to get the Lock until giving up.
     *  @return true if the Lock was acquired, false if it timed out.
     */
    bool lock(TickType_t Timeout = portMAX_DELAY) {
      return true;
    }

    /**
     *  Unlock the Mutex.
     *
     *  @return true if the Lock was released, false if it failed. (Hint,
     *           if it fails, did you call Lock() first?)
     */
    bool unlock() {
      return true;
    }
  };

#endif

#if (configUSE_RECURSIVE_MUTEXES == 1)

  /**
   *  Recursive usage Mutex.
   *
   *  By default calls to Lock these objects block forever, but this can be
   *  changed by simply passing in a argument to the Lock() method.
   *  These objects are recursively acquirable. Calling Lock() twice from
   *  the same Thread (i.e. task) works fine. The caller just needs to be sure to
   *  call Unlock() as many times as Lock().
   *
   *  @note Recursive mutexes use more resources than standard mutexes. You
   *        should be sure that you actually need this type of synchronization
   *        before using it.
   */
  class MutexRecursive {

      /////////////////////////////////////////////////////////////////////////
      //
      //  Public API
      //
      /////////////////////////////////////////////////////////////////////////
    public:
      /**
       *  Create a recursize Mutex.
       */
      MutexRecursive();

      /**
       *  Lock the Mutex.
       *
       *  @param Timeout How long to wait to get the Lock until giving up.
       *  @return true if the Lock was acquired, false if it timed out.
       */
      bool lock(TickType_t Timeout = portMAX_DELAY);

      /**
       *  Unlock the Mutex.
       *
       *  @return true if the Lock was released, false if it failed. (Hint,
       *           if it fails, did you call Lock() first?)
       */
      bool unlock();

    private:
      SemaphoreHandle_t handle;
      #if(configSUPPORT_STATIC_ALLOCATION == 1)
      StaticSemaphore_t mutexBuffer{};
      #endif
  };

#endif

  /**
   *  Synchronization helper class that leverages the C++ language to help
   *  prevent deadlocks.
   *  This is a C++11 feature that allows Mutex Locking and Unlocking to behave
   *  following an RAII style. The constructor of this helper object locks the
   *  Mutex. The destructor unlocks the Mutex. Since C++ guarantees that an
   *  object's desctuctor is always called when it goes out of scope, calls to
   *  Unlock become unnecessary and are in fact guaranteed as long as correct
   *  scoping is used.
   */
  template<class mutex_class>
  class LockGuard {

      /////////////////////////////////////////////////////////////////////////
      //
      //  Public API
      //
      /////////////////////////////////////////////////////////////////////////
    public:

      /**
       * Create a LockGuard with a specific Mutex.
       * @param m: The Mutex will be locked.
       * @param lockAcquired: bool
       * @param Timeout: How long to wait to get the Lock until giving up.
       */
      explicit LockGuard(mutex_class& m, TickType_t Timeout = portMAX_DELAY)
          :m_mutex(m)
      {
        lock_acquired = m.lock(Timeout);
      }

      /**
       *  Destroy a LockGuard.
       *
       *  @post The Mutex will be unlocked.
       */
      ~LockGuard()
      {
        if (lock_acquired) {
          m_mutex.unlock();
        }
      }

      /**
       * @return states if lock was acquired
       */
      bool lockAcquired(void)
      {
        return lock_acquired;
      }

      /////////////////////////////////////////////////////////////////////////
      //
      //  Private API
      //
      /////////////////////////////////////////////////////////////////////////
    private:

      /**
       *  Reference to the Mutex we locked, so it can be unlocked
       *  in the destructor.
       */
      mutex_class& m_mutex;
      bool lock_acquired = false;
  };

} /* namespace freertos */

#endif /* LIB_FREERTOS_CPP_MUTEX_HPP_ */
