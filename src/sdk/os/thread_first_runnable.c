#include "types.h"
#include "sdk/os.h"

OSThread* Thread_FirstRunnable(void) {
  OSThread* thread = OSi_ThreadInfo.list;

  while (thread != NULL && thread->state != OS_THREAD_STATE_READY) {
    thread = thread->next;
  }
  return thread;
}
