#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_022a7b80.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern Unk_022a7b80 data_022a7b80;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* In Survival, runs with a funds budget (modes 1 and 4) give player 1 the
 * funds left from the saved run. */
void Survival_LoadFunds(void) {
  if (STATE->gameMode != GAME_MODE_SURVIVAL) {
    return;
  }
  if (data_022a7b80.mode == 1 || data_022a7b80.mode == 4) {
    STATE->players[1].funds = data_022a7b80.budgetLeft;
  }
}
