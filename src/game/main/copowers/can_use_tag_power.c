#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

BOOL Player_IsPowerActive(int player);
BOOL CanUseSuperPower(int player, int slot);

/* TRUE when the player has a back CO, no power is on, and both COs have
 * enough charge for their Super CO Power. */
BOOL CanUseTagPower(int player) {
  u32 hasBack; /* u32 (not BOOL) keeps the 0/1 normalisation of the original */

  if (Player_IsPowerActive(player)) {
    return FALSE;
  }
  hasBack = data_027c027c->unk_48[data_027c0284]->players[player].back.id != 0;
  if (!hasBack) {
    return FALSE;
  }
  if (CanUseSuperPower(player, 0)) {
    if (CanUseSuperPower(player, 1)) {
      return TRUE;
    }
  }
  return FALSE;
}
