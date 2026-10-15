#include "types.h"
#include "game/screen.h"

void Screen_SetLayerPriorities(void);

void func_020f9564(void) {
  Screen_SetLayerPriorities();
  Screen_ResetBlend();
}
