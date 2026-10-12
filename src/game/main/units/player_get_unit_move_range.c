#include "types.h"

int GetCoMoveBonus(int player, int coSlot, int powerState, int unitType);
int UnitType_GetMovement(int unitType);

/* Movement of a unit type for a player: the base value plus the front CO's
 * bonus with no power active. */
int Player_GetUnitMoveRange(int player, int unitType) {
  int bonus = GetCoMoveBonus(player, 0, 0, unitType);

  return bonus + UnitType_GetMovement(unitType);
}
