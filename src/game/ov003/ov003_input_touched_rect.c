#include "types.h"
#include "game/ov003_unit.h"
#include "game/touch.h"

/* 1 when the screen was just touched (TOUCH_PRESSED) inside the rectangle. */
int Ov003_InputTouchedRect(Ov003Input* input, int x, int y, int width,
                           int height) {
  if (input->phase != TOUCH_PRESSED || input->active == 0) {
    return 0;
  }
  if (input->x >= x && input->x < x + width && input->y >= y &&
      input->y < y + height) {
    return 1;
  }
  return 0;
}
