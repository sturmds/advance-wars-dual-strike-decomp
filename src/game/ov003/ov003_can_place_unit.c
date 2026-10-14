#include "types.h"
#include "game/ov003_unit.h"

extern Ov003Unit data_ov003_02369538[];
extern u8
    data_ov003_02365d10[]; /* per tile code: bits 5-7 row, bits 0-4 terrain */

int Ov003_GetMoveSpeed(int player, int kind, int terrain);

/* Returns 1 when a unit of the given tile code can be placed on map tile
 * (x, y): the tile is on the map, Ov003_GetMoveSpeed gives its terrain a
 * nonzero value for that code and no live unit overlaps the tile (positions
 * are 128 per tile). Needed to match: the lower bounds reuse x and y. */
int Ov003_CanPlaceUnit(int tile, int x, int y) {
  int top;
  int maxX;
  int maxY;
  int i;
  int left;
  Ov003Unit* unit;

  unit = data_ov003_02369538;

  if (x < 0 || y < 0 || x >= data_ov003_02364a88->width ||
      y >= data_ov003_02364a88->height) {
    return 0;
  }
  if (Ov003_GetMoveSpeed(
          0, tile,
          data_ov003_02365d10[data_ov003_02364a88
                                  ->cells[y * data_ov003_02364a88->width + x]] &
              31) == 0) {
    return 0;
  }
  left = x << 7;
  top = y << 7;
  maxX = left + 112;
  maxY = top + 112;
  x = left + 16;
  y = top + 16;
  for (i = 0; i < 16; i++, unit++) {
    if (unit->hp != 0) {
      s16 unitX = unit->x;
      s16 unitY;
      if (unitX + 48 >= x) {
        unitY = unit->y;
        if (unitY + 48 >= y && unitX - 48 <= maxX && unitY - 48 <= maxY) {
          return 0;
        }
      }
    }
  }
  return 1;
}
