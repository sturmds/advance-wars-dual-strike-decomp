#include "types.h"
#include "game/map_unit.h"
#include "game/player.h"
#include "game/unit_stats.h"
#include "game/Unk_02080440.h"
#include "game/math.h"

void Player_SpendFunds(int playerIndex, int amount);

/* HP as shown on the map: 1-10, rounded up. */
#define SHOWN_HP(unit) ((unit)->hp != 0 ? HwDiv32((unit)->hp - 1, 10) + 1 : 0)

/* Fills the unit's ammo to its type's maximum, one point at a time. Paying
 * costs the type's ammoCost times the shown HP per point, otherwise the cost
 * counted is the shown HP. With payCost the current player pays and the
 * refill stops when funds run short. Returns the total cost. */
int Unit_Rearm(MapUnit* unit, BOOL payCost) {
  u16 costPerAmmo;
  int total;

  total = 0;
  if (payCost) {
    costPerAmmo = Ov000_gUnitStats[unit->type].ammoCost * SHOWN_HP(unit);
  } else {
    costPerAmmo = SHOWN_HP(unit);
  }

  for (;;) {
    if (unit->ammo == Ov000_gUnitStats[unit->type].ammo) {
      break;
    }
    if (payCost) {
      if ((u32)MAP_STATE->players[MAP_STATE->currentPlayer].funds <
          costPerAmmo) {
        break;
      }
      Player_SpendFunds(MAP_STATE->currentPlayer, costPerAmmo);
    }
    total += costPerAmmo;
    unit->ammo++;
  }
  return total;
}
