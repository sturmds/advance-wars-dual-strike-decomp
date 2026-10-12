#include "types.h"
#include "game/Unk_0216e2b0.h"
#include "game/window_wipe.h"

/* Fills the WIN0H table from the wipe progress. Each line's window grows
 * from the left edge on even lines and from the right edge on odd ones; the
 * middle rows start 48 pixels ahead of the top and bottom rows. A full line
 * (256 pixels wide) gets 0x1ff, and unk_02 and unk_03 end up as the topmost
 * and bottommost full lines of the band around the middle. */
void WindowWipe_BuildCenterOut(void) {
  int line;
  int width;
  u16* table =
      gWipeWin0hTable; /* needed to match: progress is read again each line */

  data_0216e2b0.unk_02 = 95;
  data_0216e2b0.unk_03 = 96;
  for (line = 95; line >= 0; line--) {
    width = data_0216e29c + ((line + 1) >> 1) - 96;
    if (width <= 0) {
      width = 0;
    }
    if (width >= 256) {
      width = 256;
    }
    if (line & 1) {
      table[line] = ((256 - width) & 0xff) << 8;
      if (width == 256) {
        data_0216e2b0.unk_02 = line;
        table[line] = 0x1ff;
      }
    } else {
      table[line] = width & 0xff;
      if (width == 256) {
        data_0216e2b0.unk_02 = line;
        table[line] = 0x1ff;
      }
    }
  }
  for (line = 96; line < WIPE_LINES; line++) {
    width = data_0216e29c - ((line + 1) >> 1);
    if (width <= 0) {
      width = 0;
    }
    if (width >= 256) {
      width = 256;
    }
    if (line & 1) {
      table[line] = ((256 - width) & 0xff) << 8;
      if (width == 256) {
        data_0216e2b0.unk_03 = line;
        table[line] = 0x1ff;
      }
    } else {
      table[line] = width & 0xff;
      if (width == 256) {
        data_0216e2b0.unk_03 = line;
        table[line] = 0x1ff;
      }
    }
  }
}
