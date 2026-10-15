#include "types.h"
#include "game/Unk_02080440.h"
#include "game/player.h"
#include "game/ai/ai_unload.h"

BOOL Player_HasHq(int player);

#define PLAYER (MAP_STATE->players[MAP_STATE->currentPlayer])
#define HQ_COORD_MASK 0x7f

/* Tile test for AiSeaTransport_MoveLoaded's order 6 routine: TRUE when the
 * transport's own target is not reachable, the current player has an HQ and
 * the range map reaches it. Which unit the map was filled for is a guess
 * (the cargo unit). */
BOOL AiSeaTransport_CargoReachesHq(void) {
  if (AiUnit_HasReachableTarget() == TRUE) {
    return FALSE;
  }
  if (!Player_HasHq(MAP_STATE->currentPlayer)) {
    return FALSE;
  }
  return MAP_STATE->rangeMap[PLAYER.hqY & HQ_COORD_MASK]
                            [PLAYER.hqX & HQ_COORD_MASK] > 0;
}
