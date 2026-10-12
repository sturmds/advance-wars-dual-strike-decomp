#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Nonzero when the acting unit's move ends on a Comm Tower owned by player 1
 * or player 2. */
BOOL Ov001_IsMoveDestComTowerOfPlayer1Or2(void) {
  int tile =
      STATE->mapTiles->tiles[STATE->actingUnitDestX +
                             STATE->mapTiles->rowStart[STATE->actingUnitDestY]];
  int owner;

  if (MAP_TILE_TERRAIN(tile) != TERRAIN_COM_TOWER) {
    return FALSE;
  }
  owner = MAP_TILE_OWNER(tile);
  if (owner != 1 && owner != 2) {
    return FALSE;
  }
  return TRUE;
}
