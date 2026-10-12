#include "types.h"
#include "game/menu_state.h"
#include "game/screen_other_map.h"

BOOL Skills_AreDisabled(void);
int BattleSetup_IsCursorOnCo(void);
void BattleSetup_DrawSkills(int x, int y, int set);
int Gfx_RequestLayerCopy(int layer);

extern MenuState gMenuState;

/* Needed to match: the cast keeps the field offset out of the literal pool. */
#define MENU ((MenuState*)&gMenuState)

/* Redraws the skill window (at tile 10, 19) on the screen of data_027c0288.unk_00 for
 * the CO under the cursor, unless skills are disabled. */
void VsSetup_RedrawSkills(void) {
  Unk_027c0280* saved;
  int cursor;

  if (Skills_AreDisabled() == 1) {
    return;
  }
  saved = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  if (BattleSetup_IsCursorOnCo() == 1) {
    cursor = MENU->setup.cursorButton - 8;
    BattleSetup_DrawSkills(10, 19,
                           MENU->setup.forceRankSets[cursor >> 1][cursor & 1]);
  } else {
    BattleSetup_DrawSkills(10, 19, 0);
  }
  Gfx_RequestLayerCopy(0);
  Gfx_RequestLayerCopy(1);
  Screen_SetCurrentGfx(saved);
}
