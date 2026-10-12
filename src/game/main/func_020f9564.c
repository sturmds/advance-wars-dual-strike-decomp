#include "types.h"

void Screen_SetLayerPriorities(void);
void Screen_ResetBlend(void);

void func_020f9564(void) {
  Screen_SetLayerPriorities();
  Screen_ResetBlend();
}
