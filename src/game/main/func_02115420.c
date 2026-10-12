#include "types.h"

void CARDi_LockResource(int, int);
void func_0210cff8(int a0);

void func_02115420(int a0) {
  CARDi_LockResource(a0, 1);
  func_0210cff8(a0);
}
