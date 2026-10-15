#include "types.h"
#include "game/Unk_02080440.h"

void Player_AddFunds(int playerIndex, int amount);

/* data_027c027c->unk_48[data_027c0284], written so that the compiler loads
 * the index first, as the original does. */
#define STATE                                                             \
  (*(Unk_02080440_48**)((u8*)(data_027c0284 * sizeof(Unk_02080440_48*)) + \
                            (u32)data_027c027c + (u32) &                  \
                        ((Unk_02080440*)0)->unk_48))

/* Adds the current player's income to their funds, from day 1 on. */
void CurrentPlayer_CollectIncome(void) {
  Unk_02080440_48* state = STATE;

  if (state->day != 0) {
    Player_AddFunds(state->currentPlayer,
                    state->players[state->currentPlayer].unk_08);
  }
}
