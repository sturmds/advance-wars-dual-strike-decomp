#include "types.h"

int Screen_ResetWindows(void);
void Screen_ResetBlend(void);
int func_02031480(int);

int func_020d69e4(int a0, int a1) {
  int t0 = Screen_ResetWindows();
  Screen_ResetBlend();
  return func_02031480(a1);
}
