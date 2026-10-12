#include "types.h"
#include "game/Unk_02080440.h"
#include "game/player.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

BOOL AiUnit_HasReachableTarget(void);
BOOL Player_HasHq(int player);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define PLAYER (STATE->players[STATE->currentPlayer])
#define HQ_COORD_MASK 0x7f

/* Tile test for AiSeaTransport_MoveLoaded's order 6 routine: TRUE when the
 * transport's own target is not reachable, the current player has an HQ and
 * the range map reaches it. Which unit the map was filled for is a guess
 * (the cargo unit). */
BOOL AiSeaTransport_CargoReachesHq(void) {
  if (AiUnit_HasReachableTarget() == TRUE) {
    return FALSE;
  }
  if (!Player_HasHq(STATE->currentPlayer)) {
    return FALSE;
  }
  return STATE->rangeMap[PLAYER.hqY & HQ_COORD_MASK]
                        [PLAYER.hqX & HQ_COORD_MASK] > 0;
}
