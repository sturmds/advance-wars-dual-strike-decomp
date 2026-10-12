#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])
#define CURRENT_PLAYER (STATE->players[STATE->currentPlayer])

/* Battle menu entry test: 1 hides the entry. Needs a back CO. Shown while no
 * tag power runs; its handler (Menu_SwapCo) swaps front and back CO. */
s8 BattleMenu_SwapCoHidden(void) {
  if (CURRENT_PLAYER.back.id == 0) {
    return 1;
  }
  return CURRENT_PLAYER.tagPowerHalf != 0;
}
