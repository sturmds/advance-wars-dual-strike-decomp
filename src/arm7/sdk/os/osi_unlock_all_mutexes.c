#include "types.h"
#include "sdk/os_arm7.h"

/* ARM7 copy of Mutex_ReleaseAllHeld; main already has that symbol. Releases every mutex the thread holds and wakes the threads waiting on them. */
void Mutex_ReleaseAllHeld_Arm7(OSThread* thread) {
  OSMutex* mutex;

  while (thread->mutexQueue.head != NULL) {
    mutex = Mutex_PopOldestHeld_Arm7(thread);
    mutex->count = 0;
    mutex->thread = NULL;
    Thread_ReadyAllWaiters_Arm7(&mutex->queue);
  }
}
