#include "types.h"
#include "sdk/os_arm7.h"

void Mutex_Clear_Arm7(OSMutex* mutex) {
  mutex->queue.head = 0;
  mutex->thread = NULL;
  mutex->count = 0;
}
