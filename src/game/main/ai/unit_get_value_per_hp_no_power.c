#include "types.h"

s32 HwDiv32(s32 num, s32 den);
int UnitType_GetCostDiv10(int unitType);
int Player_GetCoCostPercent(int player, int coSlot, int powerState,
                            int unitType);

/* Like Unit_GetValuePerHp, but for a given CO slot and with that CO's power
 * state taken as 0 (no power active). */
int Unit_GetValuePerHpNoPower(int player, int coSlot, int unitType) {
  int cost = UnitType_GetCostDiv10(unitType);

  return HwDiv32(
      cost * (Player_GetCoCostPercent(player, coSlot, 0, unitType) + 100), 100);
}
