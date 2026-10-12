#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

int Unit_GetValuePerHp(int playerIndex, int unitType);
void Player_SpendFunds(int playerIndex, int amount);
MapUnit* Unit_SpawnWaited(
    s16 x, s16 y, s16 unitType); /* Unit_Create, then marks the unit waited */

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u32 data_02290a94
    []; /* in gPlayRecords (+0x37c); [50 + unit type]: units bought by human players */

#define STATE (data_027c027c->unk_48[data_027c0284])

#define BOUGHT_STAT_MAX 999999

/* The current player buys a unit of unitType at (x, y): NULL when the
 * player cannot pay or the unit cannot be made. Humans' purchases count in
 * the play records, up to 999999 per unit type. */
MapUnit* CurrentPlayer_BuyUnit(int x, int y, int unitType) {
  u32 cost;
  MapUnit* unit;

  cost = Unit_GetValuePerHp(STATE->currentPlayer, unitType) * 10;
  if ((u32)STATE->players[STATE->currentPlayer].funds < cost) {
    return NULL;
  }
  unit = Unit_SpawnWaited(x, y, unitType);
  if (unit == NULL) {
    return NULL;
  }
  Player_SpendFunds(STATE->currentPlayer, cost);
  if (STATE->players[STATE->currentPlayer].controller ==
      PLAYER_CONTROLLER_HUMAN) {
    if (data_02290a94[unitType + 50] < BOUGHT_STAT_MAX) {
      data_02290a94[unitType + 50]++;
    }
  }
  return unit;
}
