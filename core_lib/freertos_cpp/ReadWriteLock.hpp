#ifndef LIB_FREERTOS_CPP_READ_WRITE_LOCK_HPP_
#define LIB_FREERTOS_CPP_READ_WRITE_LOCK_HPP_

#include "FreeRTOS.h"
#include "semphr.h"

namespace freertos {

/**
 *  Abstract base class encapsulating a Reader/Writer lock.
 *
 *  These locks are based on mutexs and cannot be used in any way from
 *  ISR context. Likewise, these locks block indefinitely.
 *
 *  @note It is expected that an application will instantiate one of the
 *        derived classes and use that object for synchronization. It is
 *        not expected that a user or application will derive from these
 *        classes.
 */
  class ReadWriteLock {

      /////////////////////////////////////////////////////////////////////////
      //
      //  Public API
      //
      /////////////////////////////////////////////////////////////////////////
    public:
      /**
           *  Constructor
           *
           *  @throws ReadWriteLockCreateException on failure.
           */
      ReadWriteLock();

      /**
           *  Destructor
           */
      virtual ~ReadWriteLock();

      /**
           *  Take the lock as a Reader.
           *  This allows multiple reader access.
           */
      virtual void readerLock() = 0;

      /**
           *  Unlock the Reader.
           */
      virtual void readerUnlock() = 0;

      /**
           *  Take the lock as a Writer.
           *  This allows only one thread access.
           */
      virtual void writerLock() = 0;

      /**
           *  Unlock the Writer.
           */
      virtual void writerUnlock() = 0;

      /////////////////////////////////////////////////////////////////////////
      //
      //  Protected API
      //  Not intended for use by application code.
      //
      /////////////////////////////////////////////////////////////////////////
    protected:
      /**
           *  How many active readers are there.
           */
      int readCount;

      /**
           *  Protect ReadCount.
           */
      SemaphoreHandle_t readLock;

      /**
           *  Protect this resource from multiple writer access, or
           *  from Reader access when a writer is changing something.
           */
      SemaphoreHandle_t resourceLock;

      #if(configSUPPORT_STATIC_ALLOCATION == 1)
      StaticSemaphore_t readBuffer{};
      StaticSemaphore_t resourceBuffer{};
      #endif
  };

/**
 *  Concrete derived class that implements a Reader/Writer lock
 *  that favors the Readers. That is, with enough aggressive readers,
 *  a Writer may starve.
 */
  class ReadWriteLockPreferReader : public ReadWriteLock {

      /////////////////////////////////////////////////////////////////////////
      //
      //  Public API
      //
      /////////////////////////////////////////////////////////////////////////
    public:
      /**
           *  Take the lock as a Reader.
           *  This allows multiple reader access.
           */
      void readerLock() final;

      /**
           *  Unlock the Reader.
           */
      void readerUnlock() final;

      /**
           *  Take the lock as a Writer.
           *  This allows only one thread access.
           */
      void writerLock() final;

      /**
           *  Unlock the Writer.
           */
      void writerUnlock() final;
  };

/**
 *  Concrete derived class that implements a Reader/Writer lock
 *  that favors the Writers. That is, with enough aggressive writers,
 *  a Reader may starve.
 */
  class ReadWriteLockPreferWriter : public ReadWriteLock {

      /////////////////////////////////////////////////////////////////////////
      //
      //  Public API
      //
      /////////////////////////////////////////////////////////////////////////
    public:
      /**
           *  Our derived constructor.
           */
      ReadWriteLockPreferWriter();

      /**
           *  Our derived destructor.
           */
      ~ReadWriteLockPreferWriter() override;

      /**
           *  Take the lock as a Reader.
           *  This allows multiple reader access.
           */
      void readerLock() final;

      /**
           *  Unlock the Reader.
           */
      void readerUnlock() final;

      /**
           *  Take the lock as a Writer.
           *  This allows only one thread access.
           */
      void writerLock() final;

      /**
           *  Unlock the Writer.
           */
      void writerUnlock() final;

      /////////////////////////////////////////////////////////////////////////
      //
      //  Private API
      //  The internals of this wrapper class.
      //
      /////////////////////////////////////////////////////////////////////////
    private:
      /**
           *  Number of Writers waiting for the Resource Lock, including any
           *  current Writer already holdign it.
           */
      int writeCount;

      /**
           *  Protect writeCount.
           */
      SemaphoreHandle_t writeLock;

      /**
           *  Lock to stop reader threads from starving a Writer.
           */
      SemaphoreHandle_t blockReadersLock;

      #if(configSUPPORT_STATIC_ALLOCATION == 1)
      StaticSemaphore_t writeBuffer{};
      StaticSemaphore_t readerBuffer{};
      #endif
  };
} // namespace freertos

#endif


