#include "types.h"
#include "game/map_object.h"
#include "game/unit_stats.h"

BOOL func_020fa6f8(void);

/* Damage chart column for a target kind (0-13: map objects, see
 * MapObject.kind, and MAP_OBJECT_TILE for a tile with hp). The Grand Bolt weak points use
 * the Oozium column; the cannons pick OBJECT_A or OBJECT_B from a game-state
 * callback (func_020fa6f8). */
int TargetKind_GetChartColumn(int target) {
  int column = 0;

  switch (target) {
    case MAP_OBJECT_LASER:
    case MAP_OBJECT_BLACK_CRYSTAL:
    case MAP_OBJECT_BLACK_OBELISK:
      column = DAMAGE_COLUMN_OBJECT_A;
      break;
    case MAP_OBJECT_GRAND_BOLT_WEAK_POINT:
    case MAP_OBJECT_GRAND_BOLT_WEAK_POINT + 1:
    case MAP_OBJECT_GRAND_BOLT_WEAK_POINT + 2:
      column = UNIT_OOZIUM;
      break;
    case MAP_OBJECT_BLACK_CANNON:
    case MAP_OBJECT_MINICANNON:
      if (func_020fa6f8() == 0) {
        column = DAMAGE_COLUMN_OBJECT_A;
      } else {
        column = DAMAGE_COLUMN_OBJECT_B;
      }
      break;
    case MAP_OBJECT_TILE:
      column = DAMAGE_COLUMN_TILE;
      break;
  }
  return column;
}
