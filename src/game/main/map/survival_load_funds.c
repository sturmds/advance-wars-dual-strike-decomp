#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_022a7b80.h"

extern Unk_022a7b80 data_022a7b80;

/* In Survival, runs with a funds budget (modes 1 and 4) give player 1 the
 * funds left from the saved run. */
void Survival_LoadFunds(void) {
  if (MAP_STATE->gameMode != GAME_MODE_SURVIVAL) {
    return;
  }
  if (data_022a7b80.mode == 1 || data_022a7b80.mode == 4) {
    MAP_STATE->players[1].funds = data_022a7b80.budgetLeft;
  }
}
