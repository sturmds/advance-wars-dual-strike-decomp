#include "types.h"
#include "game/Unk_02080440.h"
#include "game/player.h"
#include "game/ai/ai_unload.h"

BOOL Player_IsInGame(u8 player);
BOOL Player_HasHq(int player);

#define PLAYER(i) (MAP_STATE->players[i])
#define HQ_COORD_MASK 0x7f

/* Tile test for AiSeaTransport_MoveLoaded's order 2 routine: TRUE when the
 * transport's own target is not reachable and the range map reaches the HQ
 * of a player on another team. */
BOOL AiSeaTransport_CargoReachesEnemyHq(void) {
  int player;

  if (AiUnit_HasReachableTarget() == TRUE) {
    return FALSE;
  }
  for (player = 1; player < ARMY_SLOT_COUNT; player++) {
    if (!Player_IsInGame(player)) {
      continue;
    }
    if (PLAYER(MAP_STATE->currentPlayer).team == PLAYER(player).team) {
      continue;
    }
    if (!Player_HasHq(player)) {
      continue;
    }
    if (MAP_STATE->rangeMap[PLAYER(player).hqY & HQ_COORD_MASK]
                           [PLAYER(player).hqX & HQ_COORD_MASK] > 0) {
      return TRUE;
    }
  }
  return FALSE;
}
