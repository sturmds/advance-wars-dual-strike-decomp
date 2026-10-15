#include "types.h"
#include "game/screen_other_map.h"
#include "game/screen.h"

void Gfx_LoadPlayerWindowFrame(int player);

#define STATE_50 (data_027c027c->unk_50[data_027c0284])

/* Script step of the day banner's early-exit script (the one DayBanner_CheckInput
 * starts): puts layer 0 back to its normal size, runs the screen's own hook
 * if it has one, and reloads the current player's window frame graphics. */
void DayBanner_RestoreScreen(void) {
  Screen_SetLayerScreenSize(0, 0);
  if (STATE_50->unk_38 != NULL) {
    STATE_50->unk_38();
  }
  Gfx_LoadPlayerWindowFrame(MAP_STATE->currentPlayer);
}
