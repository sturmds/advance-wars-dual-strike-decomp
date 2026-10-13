#include "types.h"
#include "sdk/os.h"

/* Returns the first thread id with no thread, or -1 when all 16 are taken. */
int OSi_GetUnusedThreadId(void) {
  int id;

  for (id = 0; id < 16; id++) {
    if (OSi_ThreadInfo.threads[id] == NULL) {
      return id;
    }
  }
  return -1;
}
