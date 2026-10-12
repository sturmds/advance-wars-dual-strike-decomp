#include "types.h"

int UnitType_GetChargePerHp(int unitType); /* UnitStats.chargePerHp */

/* Power meter charge for each shown HP a unit of unitType loses. player is
 * not read. */
int Player_GetChargePerHp(int player, int unitType) {
  return UnitType_GetChargePerHp(unitType);
}
