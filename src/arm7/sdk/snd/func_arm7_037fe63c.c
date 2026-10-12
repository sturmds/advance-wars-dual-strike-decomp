#include "types.h"

int func_arm7_037fe63c(void) {
  u8 t0 = *((u8*)0x4000501);
  int t1 = t0 & -129;
  *((u8*)0x4000501) = t1;
  return t1;
}
