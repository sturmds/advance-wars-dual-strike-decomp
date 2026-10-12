#include "types.h"
#include "game/break_text.h"

/* Screen x of a letter: its x in the line plus the offset that centres the
 * line on a 256-pixel screen, wrapped to 0-255. */
u8 BreakText_GetScreenX(u8 index) {
  return (gBreakText->letterX[index] + ((256 - gBreakText->width) >> 1)) & 0xff;
}
