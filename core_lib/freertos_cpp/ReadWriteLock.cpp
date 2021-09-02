#include "ReadWriteLock.hpp"

using namespace freertos;

ReadWriteLock::ReadWriteLock()
    : readCount(0) {
  readLock = xSemaphoreCreateMutex();
  if (readLock == NULL) {
    configASSERT(!"ReadWriteLock Constructor Failed");
  }

  //
  //  resourceLock CANNOT be a mutex. In FreeRTOS, as in most OS's,
  //  a thread is not allowed to unlock another thread's mutex. But
  //  the very nature of a Reader Lock allows an arbitrary ordering
  //  of unlocks when multiple threads hold the reader lock.
  //  Semaphores are not subject to this constraint.
  //
  resourceLock = xSemaphoreCreateBinary();
  if (resourceLock == NULL) {
    vSemaphoreDelete(readLock);
    configASSERT(!"ReadWriteLock Constructor Failed");
  }

  //
  //  Initialize it as "full", so it behaves similar to a mutex.
  //
  xSemaphoreGive(resourceLock);
}

ReadWriteLock::~ReadWriteLock() {
  vSemaphoreDelete(readLock);
  vSemaphoreDelete(resourceLock);
}

void ReadWriteLockPreferReader::readerLock() {
  xSemaphoreTake(readLock, portMAX_DELAY);

  readCount++;
  if (readCount == 1) {
    xSemaphoreTake(resourceLock, portMAX_DELAY);
  }

  xSemaphoreGive(readLock);
}

void ReadWriteLockPreferReader::readerUnlock() {
  xSemaphoreTake(readLock, portMAX_DELAY);

  readCount--;
  if (readCount == 0) {
    xSemaphoreGive(resourceLock);
  }

  xSemaphoreGive(readLock);
}

void ReadWriteLockPreferReader::writerLock() {
  xSemaphoreTake(resourceLock, portMAX_DELAY);
}

void ReadWriteLockPreferReader::writerUnlock() {
  xSemaphoreGive(resourceLock);
}

ReadWriteLockPreferWriter::ReadWriteLockPreferWriter()
    : ReadWriteLock(),
      writeCount(0) {
  writeLock = xSemaphoreCreateMutex();
  if (writeLock == NULL) {
    configASSERT(!"ReadWriteLockPreferWriter Constructor Failed");
  }

  //
  //  blockReadersLock CANNOT be a mutex. In FreeRTOS, as in most OS's,
  //  a thread is not allowed to unlock another thread's mutex. But
  //  the very nature of a Reader Lock allows an arbitrary ordering
  //  of unlocks when multiple threads hold the reader lock.
  //  Semaphores are not subject to this constraint.
  //
  blockReadersLock = xSemaphoreCreateBinary();

  if (blockReadersLock == NULL) {
    vSemaphoreDelete(writeLock);
    configASSERT(!"ReadWriteLockPreferWriter Constructor Failed");
  }

  //
  //  Initialize it as "full", so it behaves similar to a mutex.
  //
  xSemaphoreGive(blockReadersLock);
}

ReadWriteLockPreferWriter::~ReadWriteLockPreferWriter() {
  vSemaphoreDelete(writeLock);
  vSemaphoreDelete(blockReadersLock);
}

void ReadWriteLockPreferWriter::readerLock() {
  xSemaphoreTake(blockReadersLock, portMAX_DELAY);
  xSemaphoreTake(readLock, portMAX_DELAY);

  readCount++;
  if (readCount == 1) {
    xSemaphoreTake(resourceLock, portMAX_DELAY);
  }

  xSemaphoreGive(readLock);
  xSemaphoreGive(blockReadersLock);
}

void ReadWriteLockPreferWriter::readerUnlock() {
  xSemaphoreTake(readLock, portMAX_DELAY);

  readCount--;
  if (readCount == 0) {
    xSemaphoreGive(resourceLock);
  }

  xSemaphoreGive(readLock);
}

void ReadWriteLockPreferWriter::writerLock() {
  xSemaphoreTake(writeLock, portMAX_DELAY);

  writeCount++;
  if (writeCount == 1) {
    xSemaphoreTake(blockReadersLock, portMAX_DELAY);
  }

  xSemaphoreGive(writeLock);

  xSemaphoreTake(resourceLock, portMAX_DELAY);
}

void ReadWriteLockPreferWriter::writerUnlock() {
  xSemaphoreGive(resourceLock);

  xSemaphoreTake(writeLock, portMAX_DELAY);

  writeCount--;
  if (writeCount == 0) {
    xSemaphoreGive(blockReadersLock);
  }

  xSemaphoreGive(writeLock);
}
