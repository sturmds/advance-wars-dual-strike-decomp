#include "types.h"
#include "game/menu_state.h"
#include "game/screen_other_map.h"

void Co_QueuePictureSpriteAtTile(int x, int y, int attr, int tile, int list);
void CombatSetup_DrawRowPlayerIcons(int x, int y);
void LinkMenu_DrawSetting(int x, int y, int entry, int affine);

extern MenuState gMenuState;

/* Queues the sprites of the Combat setup row screen (link play). Main screen:
 * one picture per row with memberCount set, on a diagonal from (32, 144) stepping
 * 64 right and 16 up, palette row + 2. Sub screen: the players on each row
 * and the five link menu settings. */
void CombatSetup_QueueRowSprites(void) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedState;
  int i;
  Unk_027c0280* subSavedGfx;
  Unk_02080440* subSavedState;

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  savedState = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  for (i = 0; i < 4; i++) {
    if (gMenuState.combat.rows[i].memberCount != 0) {
      Co_QueuePictureSpriteAtTile((32 + i * 64) & 0x1ff, (144 - i * 16) & 0xff,
                                  (i + 2) << 12, 0x400 + i * 36, 1);
    }
  }
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);

  subSavedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  subSavedState = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  CombatSetup_DrawRowPlayerIcons(48, 48);
  for (i = 0; i < 5; i++) {
    LinkMenu_DrawSetting(16, 16 + i * 32, i, 0);
  }
  Screen_SetCurrent(subSavedState);
  Screen_SetCurrentGfx(subSavedGfx);
}
