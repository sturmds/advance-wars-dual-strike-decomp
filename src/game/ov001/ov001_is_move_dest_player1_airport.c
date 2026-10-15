#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"

/* Nonzero when the acting unit's move ends on an Airport owned by player 1. */
BOOL Ov001_IsMoveDestPlayer1Airport(void) {
  int tile =
      MAP_STATE->mapTiles
          ->tiles[MAP_STATE->actingUnitDestX +
                  MAP_STATE->mapTiles->rowStart[MAP_STATE->actingUnitDestY]];

  if (MAP_TILE_TERRAIN(tile) != TERRAIN_AIRPORT) {
    return FALSE;
  }
  return MAP_TILE_OWNER(tile) == 1;
}
