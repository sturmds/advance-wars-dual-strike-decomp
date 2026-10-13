#include "types.h"
#include "sdk/os.h"

void Mutex_UnlinkFromOwner(OSThread* thread, OSMutex* mutex);
void OS_WakeupThread(OSThreadQueue* queue);

/* Drops one level of nesting; the last level frees the mutex and wakes its waiters. */
void OS_UnlockMutex(OSMutex* mutex) {
  int irqState = OS_DisableInterrupts();
  OSThread* current = OSi_ThreadInfo.current;

  if (mutex->thread == current) {
    mutex->count--;
    if (mutex->count == 0) {
      Mutex_UnlinkFromOwner(current, mutex);
      mutex->thread = NULL;
      OS_WakeupThread(&mutex->queue);
    }
  }
  OS_RestoreInterrupts(irqState);
}
