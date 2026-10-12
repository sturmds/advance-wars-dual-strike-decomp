#include "types.h"

int UnitType_GetMovement(int unitType);
int GetUnitMoveBonus(int player, int unitType);

/* Movement of a unit type for a player: the base value from the unit
 * stats table plus the front CO's change. */
int GetUnitMovement(int player, int unitType) {
  int baseMove = UnitType_GetMovement(unitType);

  return baseMove + GetUnitMoveBonus(player, unitType);
}
