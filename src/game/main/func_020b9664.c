#include "types.h"
#include "game/Unk_02080440.h"
#include "game/player.h"

extern u32 gLinkPlay;

int Link_GetOwnPlayer(void);

/* FALSE when gLinkPlay is 0. Otherwise TRUE when Link_GetOwnPlayer says so
 * or the current player is not an AI player. */
BOOL func_020b9664(void) {
  if (gLinkPlay == 0) {
    return FALSE;
  }
  if (Link_GetOwnPlayer() != 0) {
    return TRUE;
  }
  return MAP_STATE->slotControllers[MAP_STATE->currentPlayer] !=
         PLAYER_CONTROLLER_AI;
}
