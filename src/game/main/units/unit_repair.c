#include "types.h"
#include "game/map_unit.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

s32 HwDiv32(s32 numerator, s32 denominator);
int Unit_GetValuePerHp(int playerIndex, int unitType);
void Player_SpendFunds(int playerIndex, int amount);

#define GAME (data_027c027c->unk_48[data_027c0284])

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
  costPerHp = Unit_GetValuePerHp(GAME->currentPlayer, unit->type);

  for (; steps != 0; steps--) {
    if (SHOWN_HP(unit) == 10) {
      break;
    }
    if (payCost) {
      /* needed to match: the original compares unsigned */
      if ((u32)GAME->players[GAME->currentPlayer].funds < costPerHp) {
        break;
      }
      Player_SpendFunds(GAME->currentPlayer, costPerHp);
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
