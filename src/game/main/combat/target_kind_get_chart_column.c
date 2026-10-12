#include "types.h"
#include "game/unit_stats.h"

BOOL func_020fa6f8(void);

/* Damage chart column for a target kind (0-13: map objects, see MapObject.kind,
 * and 6 for a tile with hp). Kinds 11-13 use the Oozium column; kinds 3 and 4
 * pick OBJECT_A or OBJECT_B from a game-state callback (func_020fa6f8). */
int TargetKind_GetChartColumn(int target) {
  int column = 0;

  switch (target) {
    case 1:
    case 9:
    case 10:
      column = DAMAGE_COLUMN_OBJECT_A;
      break;
    case 11:
    case 12:
    case 13:
      column = UNIT_OOZIUM;
      break;
    case 3:
    case 4:
      if (func_020fa6f8() == 0) {
        column = DAMAGE_COLUMN_OBJECT_A;
      } else {
        column = DAMAGE_COLUMN_OBJECT_B;
      }
      break;
    case 6:
      column = DAMAGE_COLUMN_TILE;
      break;
  }
  return column;
}
