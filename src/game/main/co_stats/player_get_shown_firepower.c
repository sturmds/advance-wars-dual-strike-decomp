#include "types.h"
#include "game/player.h"
#include "game/menu_state.h"
#include "game/Unk_02080440.h"
#include "game/screen_other_map.h"

int Player_GetFirepowerBonus(int playerIndex, int slot, int powerState,
                             int unitType);
int Player_GetSlotFirepower(int playerIndex, int slot, int powerState,
                            int unitType, int unk);

extern MenuState gMenuState;

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Firepower bonus of a player's CO slot (0 front, 1 back) for a unit type,
 * as the unit info drawn by PlayerInfo_DrawUnitStats shows it. Slot 0 uses
 * Player_GetFirepowerBonus with the front CO's power state; other slots use
 * Player_GetSlotFirepower. In a two-map battle where the other map's unk_30 is 0, a
 * back slot instead reads the front CO of the same player on the second map
 * (data_027c0288.unk_04). */
int Player_GetShownFirepower(int playerIndex, int slot, int unitType) {
  Unk_02080440_48* game = GAME;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  int firepower;

  if (game->gameMode == 3 && gMenuState.unk_0206d67c.unk_0a8 == 1) {
    firepower = Player_GetSlotFirepower(playerIndex, slot, 0, unitType, 0);
  } else if (slot == 0) {
    firepower = Player_GetFirepowerBonus(
        playerIndex, slot, game->players[playerIndex].front.powerState,
        unitType);
  } else if (game->secondMapId != 0 && OtherMap_GetUnk30() == 0) {
    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_04);
    firepower = Player_GetFirepowerBonus(
        playerIndex, 0, GAME->players[playerIndex].front.powerState, unitType);
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
  } else {
    firepower = Player_GetSlotFirepower(playerIndex, slot, 0, unitType, 0);
  }
  return firepower;
}
