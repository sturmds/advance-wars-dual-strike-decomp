#include "types.h"
#include "game/ov003_unit.h"

extern u8
    data_ov003_02365d10[]; /* per tile code: bits 5-7 row, bits 0-4 terrain */

int Ov003Unit_CanEnterTerrain(Ov003Unit* unit, u8 terrain);

#define MAP data_ov003_02364a88
#define TERRAIN_AT(x, y) \
  (data_ov003_02365d10[MAP->cells[(y) * MAP->width + (x)]] & 31)

/* Walks the L-shaped route from (fromX, fromY) to (toX, toY) by tile: along
 * column fromX to row toY, then along row toY. Returns 0 if
 * Ov003Unit_CanEnterTerrain refuses the terrain of a tile on it. The two ends and the
 * corner (fromX, toY) are not checked. The sibling Ov003_IsPathClearRowFirst
 * walks the row first. */
int Ov003_IsPathClearColumnFirst(Ov003Unit* unit, s16 fromX, s16 fromY, s16 toX,
                                 s16 toY) {
  s16 i;

  if (fromY < toY) {
    for (fromY++; fromY < toY; fromY++) {
      if (Ov003Unit_CanEnterTerrain(unit, TERRAIN_AT(fromX, fromY)) == 0) {
        return 0;
      }
    }
  } else {
    for (i = toY + 1; i < fromY; i++) {
      if (Ov003Unit_CanEnterTerrain(unit, TERRAIN_AT(fromX, i)) == 0) {
        return 0;
      }
    }
  }
  if (fromX < toX) {
    for (i = fromX + 1; i < toX; i++) {
      if (Ov003Unit_CanEnterTerrain(unit, TERRAIN_AT(i, toY)) == 0) {
        return 0;
      }
    }
  } else {
    for (i = toX + 1; i < fromX; i++) {
      if (Ov003Unit_CanEnterTerrain(unit, TERRAIN_AT(i, toY)) == 0) {
        return 0;
      }
    }
  }
  return 1;
}
