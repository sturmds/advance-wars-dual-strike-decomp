#include "types.h"

void OSi_DoUnlockByWord(int, int, int, int);

void OS_UnlockByWord(int a0, int a1, int a2) {
  OSi_DoUnlockByWord(a0, a1, a2, 0);
}
