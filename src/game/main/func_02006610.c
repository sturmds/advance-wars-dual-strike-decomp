#include "types.h"
#include "game/screen.h"

void func_02006610(void) {
  Screen_EnableEmptyWindows();
  Screen_ResetWindowRects();
  Screen_SetNibble(1);
}
