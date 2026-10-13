#include "types.h"
#include "sdk/os.h"

/* Takes the mutex if it is free or already held by the running thread. */
BOOL OS_TryLockMutex(OSMutex* mutex) {
  int irqState = OS_DisableInterrupts();
  OSThread* owner = mutex->thread;
  OSThread* current = OSi_ThreadInfo.current;
  BOOL locked;

  if (owner == NULL) {
    mutex->thread = current;
    mutex->count++;
    locked = TRUE;
  } else if (owner == current) {
    mutex->count++;
    locked = TRUE;
  } else {
    locked = FALSE;
  }
  OS_RestoreInterrupts(irqState);
  return locked;
}
