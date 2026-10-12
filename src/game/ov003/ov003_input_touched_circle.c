#include "types.h"
#include "game/ov003_unit.h"
#include "game/touch.h"

int Math_Distance(int x1, int y1, int x2, int y2);

/* 1 when the screen was just touched (TOUCH_PRESSED) within radius of the centre. */
int Ov003_InputTouchedCircle(Ov003Input* input, int centerX, int centerY,
                             int radius) {
  if (input->phase != TOUCH_PRESSED || input->active == 0) {
    return 0;
  }
  return radius >= Math_Distance(centerX, centerY, input->x, input->y);
}
