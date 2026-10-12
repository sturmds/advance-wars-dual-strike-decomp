#include "types.h"
#include "game/event_script.h"
#include "game/Unk_02080440.h"
#include "game/player.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
int Player_GetCoId(int player, int slot);

/* Script command: skips the next arg commands unless CO slot (unk_08 & 1) of
 * the player (unk_08 >> 1; 5 is the current player) has the CO id in unk_0a. */
BOOL EventScript_SkipUnlessCo(int index) {
  int player = data_022977f0[index].pc->unk_08 >> 1;
  int coSlot = data_022977f0[index].pc->unk_08 & 1;
  int coId = data_022977f0[index].pc->unk_0a & PLAYER_CO_ID_MASK;
  int skip = data_022977f0[index].pc->arg;

  if (player == 5) {
    player = data_027c027c->unk_48[data_027c0284]->currentPlayer;
  }
  if (coId != Player_GetCoId(player, coSlot)) {
    while (skip > 0) {
      data_022977f0[index].pc++;
      skip--;
    }
  }
  data_022977f0[index].pc++;
  return TRUE;
}
