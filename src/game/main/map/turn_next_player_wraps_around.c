#include "types.h"
#include "game/Unk_02080440.h"

BOOL Player_IsInGame(u8 player);

/* TRUE when the next player still in the game after the current one has a
 * lower slot, so the turn order passes the last player and wraps. */
BOOL Turn_NextPlayerWrapsAround(void) {
  u16 next = MAP_STATE->currentPlayer;

  do {
    next++;
    if (next == 5) {
      next = 1;
    }
  } while (!Player_IsInGame(next));
  return next < MAP_STATE->currentPlayer;
}
