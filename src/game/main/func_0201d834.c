#include "types.h"

int func_0201d834(int a0) {
  u16 t0 = *((u16*)0x400000a);
  int t1 = t0 & -4;
  int t2 = t1 | a0;
  *((u16*)0x400000a) = t2;
  return t2;
}
