#include "types.h"
#include "game/break_text.h"

/* Letter x in the line plus the offset that centres the line on a 256-pixel
 * screen; not wrapped (BreakText_GetScreenX wraps to 0-255). */
s16 BreakText_GetCentredX(int index) {
  return gBreakText->letterX[index] + ((256 - gBreakText->width) >> 1);
}
