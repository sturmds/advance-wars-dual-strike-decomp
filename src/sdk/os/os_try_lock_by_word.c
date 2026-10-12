#include "types.h"

void OSi_DoTryLockByWord(int, int, int, int);

void OS_TryLockByWord(int a0, int a1, int a2) {
  OSi_DoTryLockByWord(a0, a1, a2, 0);
}
