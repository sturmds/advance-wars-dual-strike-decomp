#include "types.h"
#include "game/terrain.h"
#include "game/Unk_02080440.h"
#include "game/Unk_ov006_0235f690.h"

/* Looks up the HQ position of the army that owns hqTile (an HQ tile with
 * owner 1 to 4). Returns 1 and stores it in *x and *y when the army has an
 * HQ; otherwise returns 0 and leaves them alone. */
int Ov006_GetHqPosition(int hqTile, int* x, int* y) {
  int army;
  int found = 0;

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
      return 0;
  }
  if (data_ov006_0236d640->hqX[army] >= 0 &&
      data_ov006_0236d640->hqY[army] >= 0) {
    *x = data_ov006_0236d640->hqX[army];
    found = 1;
    *y = data_ov006_0236d640->hqY[army];
  }
  return found;
}
