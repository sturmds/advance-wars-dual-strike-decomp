#include "types.h"
#include "sdk/os.h"

void OS_WakeupThread(OSThreadQueue* queue);
OSMutex* Mutex_PopOldestHeld(OSThread* thread);

/* Releases every mutex the thread holds and wakes the threads waiting on them. */
void Mutex_ReleaseAllHeld(OSThread* thread) {
  OSMutex* mutex;

  if (thread->mutexQueue.head == NULL) {
    return;
  }
  do {
    mutex = Mutex_PopOldestHeld(thread);
    mutex->count = 0;
    mutex->thread = NULL;
    OS_WakeupThread(&mutex->queue);
  } while (thread->mutexQueue.head != NULL);
}
