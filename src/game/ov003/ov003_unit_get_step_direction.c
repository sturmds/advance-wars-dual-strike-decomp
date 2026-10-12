#include "types.h"
#include "game/ov003_unit.h"

extern Ov003Unit data_ov003_02369538[16];

int Ov003_IsPathClearColumnFirst(Ov003Unit* unit, s16 fromX, s16 fromY, s16 toX,
                                 s16 toY);
int Ov003_IsPathClearRowFirst(Ov003Unit* unit, s16 fromX, s16 fromY, s16 toX,
                              s16 toY);

/* Direction for the unit to move toward its target, the unit
 * data_ov003_02369538[unit->target]: 1 up, 2 down, 3 left, 4 right, 0 none. On
 * the same tile column or row it takes the larger pixel distance. Otherwise it
 * takes the larger tile distance's axis if a route that starts along it is
 * clear (Ov003_IsPathClearRowFirst, the row first, for left and right), else the other
 * axis. Needed to match: separate dx and dy variables for the two parts. */
int Ov003Unit_GetStepDirection(Ov003Unit* unit) {
  Ov003Unit* target = &data_ov003_02369538[unit->target];
  s16 col = unit->x >> 7;
  s16 row = unit->y >> 7;
  s16 targetCol = target->x >> 7;
  s16 targetRow = target->y >> 7;
  int dx;
  int dy;
  int tileDx;
  int tileDy;

  if (col == targetCol || row == targetRow) {
    dy = unit->y - target->y;
    if (dy < 0) {
      dy = -dy;
    }
    dx = unit->x - target->x;
    if (dx < 0) {
      dx = -dx;
    }
    if (dx > dy) {
      return unit->x > target->x ? 3 : 4;
    }
    return unit->y > target->y ? 1 : 2;
  }
  tileDy = row - targetRow;
  if (tileDy < 0) {
    tileDy = -tileDy;
  }
  tileDx = col - targetCol;
  if (tileDx < 0) {
    tileDx = -tileDx;
  }
  if (tileDx > tileDy) {
    if (Ov003_IsPathClearRowFirst(unit, col, row, targetCol, targetRow) == 1) {
      return col > targetCol ? 3 : 4;
    }
    if (Ov003_IsPathClearColumnFirst(unit, col, row, targetCol, targetRow) ==
        1) {
      return row > targetRow ? 1 : 2;
    }
  } else {
    if (Ov003_IsPathClearColumnFirst(unit, col, row, targetCol, targetRow) ==
        1) {
      return row > targetRow ? 1 : 2;
    }
    if (Ov003_IsPathClearRowFirst(unit, col, row, targetCol, targetRow) == 1) {
      return col > targetCol ? 3 : 4;
    }
  }
  return 0;
}
