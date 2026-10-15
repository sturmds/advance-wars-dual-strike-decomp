#include "types.h"
#include "game/player.h"
#include "game/menu_state.h"
#include "game/Unk_02080440.h"
#include "game/screen_other_map.h"

int GetCoMoveBonus(int player, int coSlot, int powerState, int unitType);
void Screen_SetCurrentBoth(void* state);

/* Movement bonus of a player's CO for a unit type, as the unit info drawn by
 * PlayerInfo_DrawUnitStats shows it (the movement counterpart of Player_GetShownFirepower).
 * Outside the gameMode 3 case the menu's slot replaces the slot argument; slot
 * 1 uses power state 0, slot 0 the front CO's. In a two-map battle where the
 * other map's unk_30 is 0 and the menu slot is 1, the lookup runs on the second
 * map (data_027c0288.unk_04) with slot 0. */
int Player_GetShownMoveBonus(int player, int slot, int unitType) {
  Unk_02080440* saved027c;
  Unk_027c0280* saved0280;
  int powerState;
  int bonus;

  if (gMenuState.unk_0206d67c.unk_0a8 == 1 && MAP_STATE->gameMode == 3) {
    bonus = GetCoMoveBonus(player, slot, 0, unitType);
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
    bonus = GetCoMoveBonus(player, slot, powerState, unitType);
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
  }
  return bonus;
}
