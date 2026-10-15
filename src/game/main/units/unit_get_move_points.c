#include "types.h"
#include "game/Unk_02080440.h"
#include "game/math.h"

int Player_GetFuelRate(int player);
int GetUnitMovement(int player, int unitType);

/* Movement points the unit can spend this turn: its movement, or less when
 * its fuel runs out first. Like Unit_GetUsableMovement, for the unit's owner
 * instead of the current player. */
int Unit_GetMovePoints(MapUnit* unit) {
  int fuel = unit->fuel;
  int movement = GetUnitMovement(MAP_UNIT_OWNER(MAP_STATE, unit), unit->type);
  int move = HwDiv32(fuel, Player_GetFuelRate(MAP_UNIT_OWNER(MAP_STATE, unit)));

  if (move >= movement) {
    move = movement;
  }
  return move;
}
