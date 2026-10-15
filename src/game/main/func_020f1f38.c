#include "types.h"
#include "game/screen_other_map.h"
#include "game/strike_target.h"
#include "game/missile_strike_task.h"

/* Looks for a missile strike site for player 4 on the first screen. If one is
 * found, calls MissileStrike_Start with its position, also on the first screen. */
void func_020f1f38(void) {
  s16 site[2];
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;
  int found;

  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  found = Strike_FindBestSite(site, 4, 0, STRIKE_MODE_RANDOM_8HP);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
  if (found != 1) {
    return;
  }
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  MissileStrike_Start(site[0], site[1], 2, 80);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
