#include "types.h"
#include "game/ov003_unit.h"

extern u8
    data_ov003_02365d10[]; /* per tile code: bits 5-7 row, bits 0-4 terrain */

int Ov003Unit_CanEnterTerrain(Ov003Unit* unit, u8 terrain);

#define MAP data_ov003_02364a88
#define TILE_AT(x, y) ((y) * MAP->width + (x))
#define TERRAIN_AT(x, y) \
  ((u8)(data_ov003_02365d10[MAP->cells[TILE_AT(x, y)]] & 31))

/* Checks the tiles of an L-shaped path from tile (x0, y0) to (x1, y1): along
 * row y0 up to column x1, then along column x1. Neither end nor the corner
 * (x1, y0) is checked. Returns 0 as soon as Ov003Unit_CanEnterTerrain rejects the
 * terrain of a tile for the unit, else 1. Ov003_IsPathClearColumnFirst is the same with
 * the column first (suggested name: Ov003_IsPathClearColumnFirst). */
int Ov003_IsPathClearRowFirst(Ov003Unit* unit, int x0, int y0, int x1, int y1) {
  s16 x;
  s16 y;

  if (x0 < x1) {
    for (x = x0 + 1; x < x1; x++) {
      if (!Ov003Unit_CanEnterTerrain(unit, TERRAIN_AT(x, y0))) {
        return 0;
      }
    }
  } else {
    for (x = x1 + 1; x < x0; x++) {
      if (!Ov003Unit_CanEnterTerrain(unit, TERRAIN_AT(x, y0))) {
        return 0;
      }
    }
  }
  if (y0 < y1) {
    for (y = y0 + 1; y < y1; y++) {
      if (!Ov003Unit_CanEnterTerrain(unit, TERRAIN_AT(x1, y))) {
        return 0;
      }
    }
  } else {
    for (y = y1 + 1; y < y0; y++) {
      if (!Ov003Unit_CanEnterTerrain(unit, TERRAIN_AT(x1, y))) {
        return 0;
      }
    }
  }
  return 1;
}
