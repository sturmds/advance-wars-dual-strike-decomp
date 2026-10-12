#include "types.h"

void Screen_ResetBgRegisters(void);
void Screen_ResetBlend(void);
void Screen_ResetWindows(void);
void ScreenShake_ClearOffsets(void);

void func_020097a4(void) {
  Screen_ResetBgRegisters();
  Screen_ResetBlend();
  Screen_ResetWindows();
  ScreenShake_ClearOffsets();
}
