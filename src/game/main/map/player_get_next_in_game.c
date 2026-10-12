#include "types.h"

BOOL Player_IsInGame(u8 player);

/* The next player slot after player (1-4, wrapping) that is still in the
 * game. Returns player itself when no other slot qualifies. */
u16 Player_GetNextInGame(u16 player) {
  do {
    player++;
    if (player == 5) {
      player = 1;
    }
  } while (!Player_IsInGame(player));
  return player;
}
