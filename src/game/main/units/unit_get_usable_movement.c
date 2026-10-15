#include "types.h"
#include "game/Unk_02080440.h"
#include "game/math.h"

int Player_GetFuelRate(int player);
int GetUnitMovement(int player, int unitType);

/* Movement points the unit can spend this turn: its movement, or less when
 * its fuel runs out first. */
int Unit_GetUsableMovement(MapUnit* unit) {
  int move = HwDiv32(unit->fuel, Player_GetFuelRate(MAP_STATE->currentPlayer));

  if (GetUnitMovement(MAP_STATE->currentPlayer, unit->type) <= move) {
    move = GetUnitMovement(MAP_STATE->currentPlayer, unit->type);
  }
  return move;
}
