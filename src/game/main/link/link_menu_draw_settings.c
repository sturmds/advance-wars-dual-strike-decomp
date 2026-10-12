#include "types.h"
#include "game/screen_other_map.h"
#include "game/Unk_0229fb30.h"

void LinkMenu_DrawSetting(int x, int y, int entry, int affine);

extern u8 data_02160af0
    []; /* x, y of each of the 5 entries; needed to match: not const */

/* Draws the five link menu entries on the first screen at the positions of
 * data_02160af0. The selected one (data_0229fb30.unk_45b) is drawn with
 * OAM affine matrix 31, so its icon pulses. */
void LinkMenu_DrawSettings(void) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedState;
  int i;

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  savedState = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  for (i = 0; i < 5; i++) {
    if (data_0229fb30.unk_45b == i) {
      LinkMenu_DrawSetting(data_02160af0[i * 2], data_02160af0[i * 2 + 1], i,
                           31);
    } else {
      LinkMenu_DrawSetting(data_02160af0[i * 2], data_02160af0[i * 2 + 1], i,
                           0);
    }
  }
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);
}
