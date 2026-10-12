#include "types.h"

extern u16 gScreenShakeY;
extern u16 gScreenShakeX;

void ScreenShake_ClearOffsets(void) {
  gScreenShakeX = 0;
  gScreenShakeY = 0;
}
