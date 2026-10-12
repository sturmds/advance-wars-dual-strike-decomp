#include "types.h"
#include "game/terrain.h"
#include "game/Unk_02080440.h"
#include "game/Unk_ov006_0235f690.h"

extern Unk_ov006_0235f690* data_ov006_0236d640;

/* Sets the HQ position of the army that owns hqTile (an HQ tile with owner
 * 1 to 4) back to -1, meaning no HQ. Any other tile changes nothing. */
void Ov006_ClearHqPosition(int hqTile) {
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
  data_ov006_0236d640->hqX[army] = -1;
  data_ov006_0236d640->hqY[army] = -1;
}
