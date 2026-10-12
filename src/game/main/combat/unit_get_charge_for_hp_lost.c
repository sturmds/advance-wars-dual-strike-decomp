#include "types.h"

int Player_GetChargePerHp(int player, int unitType);

/* Power meter charge for hpLost shown HP of a unit of unitType. */
int Unit_GetChargeForHpLost(int player, int hpLost, int unitType) {
  int perHp = Player_GetChargePerHp(player, unitType);
  return hpLost * perHp;
}
