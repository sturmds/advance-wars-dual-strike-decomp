#include "types.h"
#include "game/event_script.h"
#include "game/Unk_02080440.h"
#include "game/player.h"

int Player_GetCoId(int player, int slot);
BOOL EventCmd_Jump(s16 index);

/* Script command: jumps (EventCmd_Jump) when the player in arg has the two COs
 * in unk_08 and unk_0a, in either order; otherwise goes on to the next
 * command. Player 5 means the current player. */
BOOL EventScript_JumpIfCos(s16 index) {
  int firstCo = data_022977f0[index].pc->unk_08 & PLAYER_CO_ID_MASK;
  int secondCo = data_022977f0[index].pc->unk_0a & PLAYER_CO_ID_MASK;
  int player = data_022977f0[index].pc->arg;

  if (player == 5) {
    player = data_027c027c->unk_48[data_027c0284]->currentPlayer;
  }
  if (firstCo == Player_GetCoId(player, 0) &&
      secondCo == Player_GetCoId(player, 1)) {
    return EventCmd_Jump(index);
  }
  if (secondCo == Player_GetCoId(player, 0) &&
      firstCo == Player_GetCoId(player, 1)) {
    return EventCmd_Jump(index);
  }
  data_022977f0[index].pc++;
  return TRUE;
}
