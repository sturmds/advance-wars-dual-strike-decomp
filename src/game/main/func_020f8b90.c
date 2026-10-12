#include "types.h"

void Screen_ResetBlend(void);
void Screen_ResetWindows(void);

void func_020f8b90(void) {
  Screen_ResetBlend();
  Screen_ResetWindows();
}
