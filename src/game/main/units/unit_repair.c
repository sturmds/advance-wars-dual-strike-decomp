#include "types.h"
#include "game/map_unit.h"
#include "game/player.h"
#include "game/Unk_02080440.h"
#include "game/math.h"

int Unit_GetValuePerHp(int playerIndex, int unitType);
void Player_SpendFunds(int playerIndex, int amount);

/* HP as shown on the map: 1-10, rounded up. */
#define SHOWN_HP(unit) ((unit)->hp != 0 ? HwDiv32((unit)->hp - 1, 10) + 1 : 0)

/* Gives the unit up to `steps` shown HP, stopping at 10. With payCost the
 * current player pays the unit's value per HP for each step, and the repair
 * stops when the funds run short. The hp is then rounded up to a whole shown
 * HP. Returns the total cost, counted even when payCost is FALSE. */
int Unit_Repair(MapUnit* unit, s16 steps, BOOL payCost) {
  u16 costPerHp;
  int total;

  total = 0;
  costPerHp = Unit_GetValuePerHp(MAP_STATE->currentPlayer, unit->type);

  for (; steps != 0; steps--) {
    if (SHOWN_HP(unit) == 10) {
      break;
    }
    if (payCost) {
      /* needed to match: the original compares unsigned */
      if ((u32)MAP_STATE->players[MAP_STATE->currentPlayer].funds < costPerHp) {
        break;
      }
      Player_SpendFunds(MAP_STATE->currentPlayer, costPerHp);
    }
    total += costPerHp;
    unit->hp += 10;
    if (unit->hp > 100) {
      unit->hp = 100;
    }
  }
  unit->hp = SHOWN_HP(unit) * 10;
  return total;
}
