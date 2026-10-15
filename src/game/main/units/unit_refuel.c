#include "types.h"
#include "game/map_unit.h"
#include "game/player.h"
#include "game/unit_stats.h"
#include "game/Unk_02080440.h"
#include "game/math.h"

void Player_SpendFunds(int playerIndex, int amount);

/* HP as shown on the map: 1-10, rounded up. */
#define SHOWN_HP(unit) ((unit)->hp != 0 ? HwDiv32((unit)->hp - 1, 10) + 1 : 0)

/* Fills the unit's fuel to its type's maximum, one point at a time. Each
 * point costs the unit's shown HP (1-10). With payCost the current player
 * pays it and the refuel stops when the funds run short. Returns the total
 * cost, counted even when payCost is FALSE. */
int Unit_Refuel(MapUnit* unit, BOOL payCost) {
  u16 costPerFuel;
  int total;

  total = 0;
  costPerFuel = SHOWN_HP(unit);

  for (;;) {
    if (unit->fuel == Ov000_gUnitStats[unit->type].fuel) {
      break;
    }
    if (payCost) {
      if ((u32)MAP_STATE->players[MAP_STATE->currentPlayer].funds <
          costPerFuel) {
        break;
      }
      Player_SpendFunds(MAP_STATE->currentPlayer, costPerFuel);
    }
    total += costPerFuel;
    unit->fuel++;
  }
  return total;
}
