#include "types.h"
#include "game/Unk_02080440.h"

BOOL Player_IsInGame(u8 player);
u8 Map_GetLoadedPlayerCount(void);

/* Returns the current player if still in the game, else the first player
 * (1 to 4) that is, else 0. */
int Player_GetCurrentOrFirstInGame(void) {
  int player;

  if (Player_IsInGame(MAP_STATE->currentPlayer)) {
    return MAP_STATE->currentPlayer;
  }
  for (player = 0; player < Map_GetLoadedPlayerCount(); player++) {
    if (Player_IsInGame(player + 1)) {
      return player + 1;
    }
  }
  return 0;
}
