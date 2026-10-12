#include "types.h"

void func_arm7_03805168(int, int, int, int);

void func_arm7_0380513c(int a0, int a1, int a2) {
  int t0 = a2 & 0xffff;
  int t1 = (a0 & 0x3c00000) << 22;
  func_arm7_03805168(t0 | (t1 | ((a1 & 0x3f0000) << 16)), a1, t1, t0);
}
