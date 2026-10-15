#include "types.h"
#include "game/Unk_02080440.h"

BOOL Player_IsInGame(u8 player);
BOOL Player_HasHq(int player); /* TRUE: bit 0x80 of Player.hqX is clear */

#define MAP_STATE (data_027c027c->unk_48[data_027c0284])

/* TRUE when some player still in the game, on another team than the
 * current player, passes Player_HasHq. */
BOOL func_0209dd7c(void) {
  int player;

  for (player = 1; player < ARMY_SLOT_COUNT; player++) {
    if (Player_IsInGame(player) &&
        MAP_STATE->players[MAP_STATE->currentPlayer].team !=
            MAP_STATE->players[player].team &&
        Player_HasHq(player) == TRUE) {
      return TRUE;
    }
  }
  return FALSE;
}
