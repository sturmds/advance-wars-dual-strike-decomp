#include "types.h"
#include "game/terrain.h"
#include "game/Unk_02080440.h"
#include "game/Unk_ov006_0235f690.h"

/* Records x, y as the HQ position of the army that owns hqTile (an HQ tile
 * with owner 1 to 4). Any other tile changes nothing. */
void Ov006_SetHqPosition(int hqTile, int x, int y) {
  int army;

  switch (hqTile) {
    case MAP_TILE_MAKE(TERRAIN_HQ, 1):
      army = 0;
      break;
    case MAP_TILE_MAKE(TERRAIN_HQ, 2):
      army = 1;
      break;
    case MAP_TILE_MAKE(TERRAIN_HQ, 3):
      army = 2;
      break;
    case MAP_TILE_MAKE(TERRAIN_HQ, 4):
      army = 3;
      break;
    default:
      return;
  }
  data_ov006_0236d640->hqX[army] = x;
  data_ov006_0236d640->hqY[army] = y;
}
