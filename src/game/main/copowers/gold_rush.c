#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

void Player_AddFunds(int playerIndex, int amount);
void Sound_PlaySe(int id);
void func_020e2670(int priority);

/* Colin's CO Power: funds x1.5. Adds half of the current funds to the
 * current player, then passes id 0xdf to Sound_PlaySe (doc 890 calls
 * it a sound) and starts the task shared with other CO Powers. */
void GoldRush(int priority) {
  Unk_02080440_48* state = data_027c027c->unk_48[data_027c0284];

  /* needed to match: unsigned shift (lsr) */
  Player_AddFunds(state->currentPlayer,
                  (u32)state->players[state->currentPlayer].funds >> 1);
  Sound_PlaySe(0xdf);
  func_020e2670(priority);
}
