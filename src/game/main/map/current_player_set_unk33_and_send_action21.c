#include "types.h"
#include "game/Unk_02080440.h"

extern u32 gLinkPlay;

void Player_SetUnk33(int player);
void UnitAction_Send(int action, int unitIndex, u8 target, u8 arg);

/* Sets Player.unk_33 on the current player. When gLinkPlay is nonzero
 * (as in func_020c0178) it also sends unit action 21. */
void CurrentPlayer_SetUnk33AndSendAction21(void) {
  Player_SetUnk33(MAP_STATE->currentPlayer);
  if (gLinkPlay == 0) {
    return;
  }
  UnitAction_Send(21, 0, 0, 0);
}
