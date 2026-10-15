#include "types.h"
#include "game/co_data.h"
#include "game/player.h"
#include "game/Unk_02080440.h"
#include "game/math.h"

void Sound_PlaySe(int id);
BOOL Player_IsInGame(u8 player);
void func_020e2670(int priority);
int SuperPowerCost(int player, int slot);

/* Sasha's CO Power: each CO (front and back) of every enemy still in the
 * game loses meter charge equal to percent of its Super cost, where
 * percent is the current player's funds / 500, capped at 100. */
void MarketCrash(int priority) {
  int percent;
  int hundred; /* needed to match: the divisor is kept on the stack */
  int charge;
  int player;
  int slot;

  Sound_PlaySe(0xe3);
  func_020e2670(priority);
  percent =
      HwDiv32(data_027c027c->unk_48[data_027c0284]
                  ->players[data_027c027c->unk_48[data_027c0284]->currentPlayer]
                  .funds,
              500);
  if (percent > 100) {
    percent = 100;
  }
  hundred = 100;
  for (player = 1; player < ARMY_SLOT_COUNT; player++) {
    if (!Player_IsInGame(player)) {
      continue;
    }
    if (data_027c027c->unk_48[data_027c0284]
            ->players[data_027c027c->unk_48[data_027c0284]->currentPlayer]
            .team ==
        data_027c027c->unk_48[data_027c0284]->players[player].team) {
      continue;
    }
    for (slot = 0; slot < PLAYER_CO_COUNT; slot++) {
      if ((&data_027c027c->unk_48[data_027c0284]->players[player].front)[slot]
              .id &
          PLAYER_CO_ID_MASK) {
        int cost;

        charge =
            (&data_027c027c->unk_48[data_027c0284]->players[player].front)[slot]
                .charge;
        cost = SuperPowerCost(player, slot);
        cost = HwDiv32(cost * percent, hundred);
        if (charge < cost) {
          charge = 0;
        } else {
          charge -= cost;
        }
        (&data_027c027c->unk_48[data_027c0284]->players[player].front)[slot]
            .charge = charge;
      }
    }
  }
}
