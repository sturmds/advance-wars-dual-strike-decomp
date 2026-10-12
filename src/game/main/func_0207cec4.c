#include "types.h"

int Link_DrawPlayerRows(void);
void func_020b9864(void);
int Task_JumpToLabel(int, int);

int func_0207cec4(int a0) {
  int t0 = Link_DrawPlayerRows();
  func_020b9864();
  return Task_JumpToLabel(a0, 255);
}
