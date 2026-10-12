#include "types.h"

void Screen_SetCurrentGfx(void);
int Screen_SetCurrent(int);

int Screen_SetCurrentBoth(int a0) {
  Screen_SetCurrentGfx();
  return Screen_SetCurrent(a0);
}
