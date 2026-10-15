#include "types.h"
#include "game/Unk_0216e2b0.h"
#include "game/window_wipe.h"
#include "game/screen_wipe.h"

/* Same as WindowWipe_BuildCenterOut, but the top and bottom rows start 48
 * pixels ahead of the middle rows. unk_02 and unk_03 end up as the last full
 * line from the top and the first full line from the bottom. */
void WindowWipe_BuildEdgesIn(void) {
  int line;
  int width;
  u16* table =
      gWipeWin0hTable; /* needed to match: progress is read again each line */

  data_0216e2b0.unk_02 = 0;
  data_0216e2b0.unk_03 = 192;
  for (line = 0; line < WIPE_LINES / 2; line++) {
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
  for (line = WIPE_LINES - 1; line >= WIPE_LINES / 2; line--) {
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
