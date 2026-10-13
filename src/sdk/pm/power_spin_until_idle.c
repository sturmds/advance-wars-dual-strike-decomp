#include "sdk/pm.h"

/* Waits until the command in flight has finished. Always returns 0 (needed to match). */
int Power_SpinUntilIdle(void) {
  int lock;

  /* Needed to match: the ARM7 reply clears lock, so each pass reads it again. */
  while ((lock = *(volatile int*)&gPMWork.lock) != 0) {
  }
  return lock;
}
