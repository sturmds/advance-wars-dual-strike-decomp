#include "types.h"
#include "game/Unk_02080440.h"
#include "game/co_data.h"
#include "game/unit_stats.h"
#include "game/move_path.h"

extern MovePath* gMovePath;
extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Owner of the acting unit: slot 0 is neutral, so unit slots 0-63
 * belong to player 1. */
#define ACTING_PLAYER (GAME->players[(GAME->actingUnitIndex >> 6) + 1])

/* Cuts the move path down to its first count tiles (when it is longer)
 * and works out the move points left on each again. */
void MovePath_Truncate(int count) {
  s8 i;
  int co;

  if (gMovePath->lastTile < count) {
    return;
  }
  gMovePath->lastTile = count - 1;
  gMovePath->movesLeft[gMovePath->lastTile] = gMovePath->startMoves;

  for (i = 1; i <= gMovePath->lastTile; i++) {
    if (GAME->coEffectsOn == 0) {
      co = 0;
    } else {
      co = ACTING_PLAYER.front.id & PLAYER_CO_ID_MASK;
    }
    gMovePath->movesLeft[i] =
        gMovePath->movesLeft[i - 1] -
        ((const s8(*)[32])gCoTable[co]
             .blocks[ACTING_PLAYER.front.powerState]
             .movementCost[ACTING_PLAYER.moveChart])
            [Ov000_gUnitStats[GAME->actingUnit->type].moveType]
            [MAP_TILE_TERRAIN(
                GAME->mapTiles
                    ->tiles[gMovePath->x[i] +
                            GAME->mapTiles->rowStart[gMovePath->y[i]]])];
  }
}
