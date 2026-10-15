#include "types.h"
#include "game/player.h"
#include "game/menu_state.h"
#include "game/Unk_02080440.h"
#include "game/screen_other_map.h"

int GetCoRangeBonus(int player, int coSlot, int powerState, int unitType);
void Screen_SetCurrentBoth(void* state);

/* Range bonus of a player's CO for a unit type, as PlayerInfo_DrawUnitStats
 * shows it. Same slot and two-map handling as Player_GetShownMoveBonus. */
int Player_GetShownRangeBonus(int player, int slot, int unitType) {
  Unk_02080440* saved027c;
  Unk_027c0280* saved0280;
  int powerState;
  int bonus;

  if (gMenuState.unk_0206d67c.unk_0a8 == 1 && MAP_STATE->gameMode == 3) {
    bonus = GetCoRangeBonus(player, slot, 0, unitType);
  } else {
    saved027c = data_027c027c;
    saved0280 = data_027c0280;
    if (MAP_STATE->secondMapId != 0 && OtherMap_GetUnk30() == 0 &&
        gMenuState.unk_0206d67c.slot == 1) {
      Screen_SetCurrentBoth(data_027c0288.unk_04);
      slot = 0;
    } else {
      slot = gMenuState.unk_0206d67c.slot;
    }
    if (slot == 1) {
      powerState = 0;
    } else {
      powerState = MAP_STATE->players[player].front.powerState;
    }
    bonus = GetCoRangeBonus(player, slot, powerState, unitType);
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
  }
  return bonus;
}
