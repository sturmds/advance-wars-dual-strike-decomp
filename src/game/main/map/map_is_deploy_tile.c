#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"

BOOL DeployCity(u8 tile);

/* TRUE when the acting player can deploy ground units on the tile: DeployCity
 * accepts it, or it is an HQ of the player whose side is current. */
BOOL Map_IsDeployTile(u8 tile) {
  if (DeployCity(tile)) {
    return TRUE;
  }
  if (MAP_TILE_TERRAIN(tile) == TERRAIN_HQ &&
      (tile & 0xe0) == data_027c027c->unk_48[data_027c0284]->unk_04) {
    return TRUE;
  }
  return FALSE;
}
