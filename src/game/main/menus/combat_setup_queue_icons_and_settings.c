#include "types.h"
#include "game/Unk_0229fb30.h"
#include "game/screen_other_map.h"

void CombatSetup_DrawRowPlayerIcons(int x, int y);
void LinkMenu_DrawSetting(int x, int y, int entry, int affine);

extern u8 data_02160af0
    []; /* x, y of each of the 5 entries; needed to match: not const */

/* Like the sub screen part of CombatSetup_QueueRowSprites, but the row player
 * icons go on the first screen, at (32, 32), and the five link menu settings
 * on the second screen at the positions of data_02160af0, 8 lower. */
void CombatSetup_QueueIconsAndSettings(void) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedState;
  int i;
  Unk_027c0280* subSavedGfx;
  Unk_02080440* subSavedState;

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  savedState = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  CombatSetup_DrawRowPlayerIcons(32, 32);
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);

  subSavedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  subSavedState = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  for (i = 0; i < LINK_SETTING_COUNT; i++) {
    LinkMenu_DrawSetting(data_02160af0[i * 2], data_02160af0[i * 2 + 1] + 8, i,
                         0);
  }
  Screen_SetCurrent(subSavedState);
  Screen_SetCurrentGfx(subSavedGfx);
}
