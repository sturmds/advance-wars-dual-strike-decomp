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

/* Appends tile (x, y) to the move path and stores the move points left
 * there: the previous tile's minus the cost of the tile's terrain for the
 * acting unit. With CO effects off, the cost comes from CO 0's charts. */
void MovePath_AddTile(s8 x, s8 y) {
  int co;

  gMovePath->lastTile++;
  gMovePath->x[gMovePath->lastTile] = x;
  gMovePath->y[gMovePath->lastTile] = y;

  if (GAME->coEffectsOn == 0) {
    co = 0;
  } else {
    co = ACTING_PLAYER.front.id & PLAYER_CO_ID_MASK;
  }
  /* Each chart holds 32 terrain costs per move type. */
  gMovePath->movesLeft[gMovePath->lastTile] =
      gMovePath->movesLeft[gMovePath->lastTile - 1] -
      ((const s8(*)[32])gCoTable[co]
           .blocks[ACTING_PLAYER.front.powerState]
           .movementCost[ACTING_PLAYER.moveChart])
          [Ov000_gUnitStats[GAME->actingUnit->type].moveType][MAP_TILE_TERRAIN(
              GAME->mapTiles->tiles[x + GAME->mapTiles->rowStart[y]])];
}
