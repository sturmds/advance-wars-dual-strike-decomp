#include "types.h"

int UnitType_GetMaxRange(int unitType);
int GetCoRangeBonus(int player, int coSlot, int powerState, int unitType);

/* Maximum firing range of a unit type under one CO of a player. */
int GetCoUnitMaxRange(int player, int coSlot, int powerState, int unitType) {
  int baseRange = UnitType_GetMaxRange(unitType);

  return baseRange + GetCoRangeBonus(player, coSlot, powerState, unitType);
}
