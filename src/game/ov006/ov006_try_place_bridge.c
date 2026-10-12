#include "types.h"
#include "game/design_map_gen.h"

void Ov006_SetMapCell(int x, int y, int cell);
BOOL func_ov006_0235b784(
    int x,
    int y); /* guess: TRUE off the map or on sea, shoal, reef or cell 19 (a bridge) */

/* Per tile, by row (30 tiles a row): 2 marks a tile on a planned road; the
 * caller lays road cells on those tiles. */

#define PLAN_ROAD 2
#define CELL_BRIDGE_ACROSS \
  22 /* both bridge cells are TERRAIN_BRIDGE (Ov006_SetMapCell) */
#define CELL_BRIDGE_DOWN 21

/* Turns road tile x, y into a bridge when it has water (by
 * func_ov006_0235b784) on both sides that is not road itself: left and right
 * first, then above and below. Returns TRUE if it placed one. */
BOOL Ov006_TryPlaceBridge(int x, int y) {
  if (func_ov006_0235b784(x - 1, y) == TRUE &&
      Ov006_gGenMarks[y][x - 1] != PLAN_ROAD &&
      func_ov006_0235b784(x + 1, y) == TRUE &&
      Ov006_gGenMarks[y][x + 1] != PLAN_ROAD) {
    Ov006_SetMapCell(x, y, CELL_BRIDGE_ACROSS);
    return TRUE;
  }
  if (func_ov006_0235b784(x, y - 1) == TRUE &&
      Ov006_gGenMarks[y - 1][x] != PLAN_ROAD &&
      func_ov006_0235b784(x, y + 1) == TRUE &&
      Ov006_gGenMarks[y + 1][x] != PLAN_ROAD) {
    Ov006_SetMapCell(x, y, CELL_BRIDGE_DOWN);
    return TRUE;
  }
  return FALSE;
}
