#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

void Player_CommitPowerState(u16 player);
void MovementCost(void);
void func_020c4018(void);

/* After a power changes the map (Blizzard): resets the scroll of layer 0's BG,
 * then runs Player_CommitPowerState for the current player, MovementCost and func_020c4018. */
void Power_ResetScrollAndRefresh(void) {
  data_027c0280->bgScrollX[data_027c0280->layer0Bg] = 0;
  data_027c0280->bgScrollY[data_027c0280->layer0Bg] = 0;
  Player_CommitPowerState(data_027c027c->unk_48[data_027c0284]->currentPlayer);
  MovementCost();
  func_020c4018();
}
