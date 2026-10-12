#include "types.h"

void Screen_EnableEmptyWindows(void);
void Screen_ResetWindowRects(void);
void Screen_SetNibble(int value);

void func_02006610(void) {
  Screen_EnableEmptyWindows();
  Screen_ResetWindowRects();
  Screen_SetNibble(1);
}
