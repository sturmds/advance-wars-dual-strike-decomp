#include "types.h"

int Screen_ResetWindows(void);
void Screen_ResetBlend(void);
int func_020f4c00(int, int);

int func_020d69c0(int a0) {
  int t0 = Screen_ResetWindows();
  Screen_ResetBlend();
  return func_020f4c00(1, a0);
}
