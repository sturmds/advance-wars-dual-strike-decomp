#include "types.h"
#include "game/menu_state.h"
#include "game/screen_other_map.h"

BOOL Skills_AreDisabled(void);
void BattleSetup_DrawSkills(int x, int y, int set);
int Gfx_RequestLayerCopy(int layer);

/* Redraws the four skill windows of the Vs setup screen (tiles 4 and 18 on
 * rows 10 and 16, force rank sets 0 to 3) on the screen of data_027c0288.unk_04,
 * unless skills are disabled. */
void VsSetup_RedrawAllSkills(void) {
  Unk_027c0280* saved;

  if (Skills_AreDisabled() == 1) {
    return;
  }
  saved = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  BattleSetup_DrawSkills(4, 10, 0);
  BattleSetup_DrawSkills(18, 10, 1);
  BattleSetup_DrawSkills(4, 16, 2);
  BattleSetup_DrawSkills(18, 16, 3);
  Gfx_RequestLayerCopy(0);
  Gfx_RequestLayerCopy(1);
  Screen_SetCurrentGfx(saved);
}
