#include "types.h"
#include "sdk/os_arm7.h"

/* ARM7 copy of Mutex_PopOldestHeld; main already has that symbol. Removes the oldest mutex from the thread's list and returns it. */
OSMutex* Mutex_PopOldestHeld_Arm7(OSThread* thread) {
  OSMutex* head = thread->mutexQueue.head;
  OSMutex* next = head->next;

  if (next == NULL) {
    thread->mutexQueue.tail = NULL;
  } else {
    next->prev = NULL;
  }
  thread->mutexQueue.head = next;
  return head;
}
