#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Nonzero when the acting unit's move ends on an Airport owned by player 1. */
BOOL Ov001_IsMoveDestPlayer1Airport(void) {
  int tile =
      STATE->mapTiles->tiles[STATE->actingUnitDestX +
                             STATE->mapTiles->rowStart[STATE->actingUnitDestY]];

  if (MAP_TILE_TERRAIN(tile) != TERRAIN_AIRPORT) {
    return FALSE;
  }
  return MAP_TILE_OWNER(tile) == 1;
}
