#include "types.h"
#include "sdk/os_arm7.h"

extern OSThreadInfo data_arm7_0380790c;

/* ARM7 copy of OSi_InsertThreadToList; main already has that symbol. */
void Thread_LinkByPriority_Arm7(OSThread* thread) {
  OSThread* cur;
  OSThread* prev;
  OSThread* head;

  head = data_arm7_0380790c.list;
  cur = head;
  prev = NULL;
  while (cur != NULL && cur->priority < thread->priority) {
    prev = cur;
    cur = cur->next;
  }
  if (prev == NULL) {
    thread->next = head;
    data_arm7_0380790c.list = thread;
  } else {
    thread->next = prev->next;
    prev->next = thread;
  }
}
