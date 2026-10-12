#include "types.h"

int Player_GetSlotFirepower(int playerIndex, int slot, int powerState,
                            int unitType, int tagPower);
int ScaleChartDamage(int firepower, int unitType, int targetType, int weapon,
                     BOOL targetDived);

/* Damage a unit type's weapon (0 primary, 1 secondary) deals to a target
 * unit type with one CO slot's firepower and no terrain or stealth bonus. */
int Co_GetSlotChartDamage(int playerIndex, int slot, int powerState,
                          int unitType, int targetType, int weapon,
                          BOOL targetDived) {
  int firepower =
      Player_GetSlotFirepower(playerIndex, slot, powerState, unitType, 0) + 100;

  return ScaleChartDamage(firepower, unitType, targetType, weapon, targetDived);
}
