#include "types.h"
#include "sdk/os_arm7.h"

extern OSThreadInfo data_arm7_0380790c;

/* ARM7 copy of OSi_GetUnusedThreadId; main already has that symbol. Returns -1 when all 16 ids are taken. */
int Thread_FirstFreeId_Arm7(void) {
  int id;

  for (id = 0; id < THREAD_ID_COUNT; id++) {
    if (data_arm7_0380790c.threads[id] == NULL) {
      return id;
    }
  }
  return -1;
}
