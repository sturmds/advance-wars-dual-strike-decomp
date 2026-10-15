#include "types.h"
#include "game/co_data.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

#define PLAYER(i) (data_027c027c->unk_48[data_027c0284]->players[i])
#define BLOCK(co, index) (gCoTable[co].blocks[index])

/* Stores `value` in the player's front CO unk_12 field, runs that CO's
 * block of that index (its activate routine with `arg`), then runs the
 * calcBonus routine of the current player's front CO block with 0. What
 * unk_12 selects beyond the block is not known; no caller was found. */
void func_020e1acc(int player, int value, int arg) {
  PLAYER(player).front.unk_12 = value;
  BLOCK(PLAYER(player).front.id & PLAYER_CO_ID_MASK,
        PLAYER(player).front.unk_12)
      .activate(arg);
  BLOCK(
      PLAYER(data_027c027c->unk_48[data_027c0284]->currentPlayer).front.id &
          PLAYER_CO_ID_MASK,
      PLAYER(data_027c027c->unk_48[data_027c0284]->currentPlayer).front.unk_12)
      .calcBonus(0);
}
