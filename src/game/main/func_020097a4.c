#include "types.h"
#include "game/screen.h"

void func_020097a4(void) {
  Screen_ResetBgRegisters();
  Screen_ResetBlend();
  Screen_ResetWindows();
  ScreenShake_ClearOffsets();
}
