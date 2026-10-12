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
      if ((u32)GAME->players[GAME->currentPlayer].funds < costPerFuel) {
        break;
      }
      Player_SpendFunds(GAME->currentPlayer, costPerFuel);
    }
    total += costPerFuel;
    unit->fuel++;
  }
  return total;
}
