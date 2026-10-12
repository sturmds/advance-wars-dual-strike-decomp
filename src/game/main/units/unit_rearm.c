#include "types.h"
#include "game/map_unit.h"
#include "game/player.h"
#include "game/unit_stats.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

s32 HwDiv32(s32 numerator, s32 denominator);
void Player_SpendFunds(int playerIndex, int amount);

#define GAME (data_027c027c->unk_48[data_027c0284])

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
      if ((u32)GAME->players[GAME->currentPlayer].funds < costPerAmmo) {
        break;
      }
      Player_SpendFunds(GAME->currentPlayer, costPerAmmo);
    }
    total += costPerAmmo;
    unit->ammo++;
  }
  return total;
}
