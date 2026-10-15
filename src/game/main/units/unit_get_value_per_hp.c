#include "types.h"
#include "game/math.h"

u32 UnitType_GetCostDiv10(int unitType);
int func_020df900(int playerIndex, int unitType);

/* Cost of one HP of a unit type in funds / 10 (the type's cost / 10), scaled
 * by the player's cost change in percent. */
int Unit_GetValuePerHp(int playerIndex, int unitType) {
  u32 baseValue = UnitType_GetCostDiv10(unitType);

  return HwDiv32(baseValue * (func_020df900(playerIndex, unitType) + 100), 100);
}
