#include "types.h"

void Gfx_LoadBmap085(void);
void func_020f28d4(int);
int func_020f29d4(int);

int func_020f2a18(int a0) {
  Gfx_LoadBmap085();
  func_020f28d4(a0);
  return func_020f29d4(a0);
}
