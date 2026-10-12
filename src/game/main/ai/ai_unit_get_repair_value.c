#include "types.h"
#include "game/map_unit.h"

s32 HwDiv32(s32 num, s32 den);
int Player_GetRepairHp(int playerIndex, int slot, int powerState);
int Unit_GetValuePerHpNoPower(int player, int coSlot, int unitType);

#define SHOWN_HP(unit) ((unit)->hp != 0 ? HwDiv32((unit)->hp - 1, 10) + 1 : 0)

/* Value in funds / 10 of the HP one repair would give the unit: the player's
 * repair amount, limited to the shown HP the unit is missing, times the value
 * per HP. 0 for a unit above 90 hp. */
int AiUnit_GetRepairValue(int player, int coSlot, MapUnit* unit) {
  int repair;
  int missing;

  if (unit->hp >= 91) {
    return 0;
  }
  repair = Player_GetRepairHp(player, coSlot, 0);
  missing = 10 - SHOWN_HP(unit);
  if (repair > missing) {
    repair = missing;
  }
  return repair * Unit_GetValuePerHpNoPower(player, coSlot, unit->type);
}
