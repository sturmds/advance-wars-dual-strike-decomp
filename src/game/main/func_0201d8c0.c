#include "types.h"

int func_0201d8c0(int a0) {
  int t0 = *((int*)0x4000000);
  int t1 = t0 & -0x38000001;
  int t2 = a0 << 27;
  int t3 = t1 | t2;
  *((int*)0x4000000) = t3;
  return t3;
}
