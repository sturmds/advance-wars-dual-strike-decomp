#include "types.h"

BOOL Tile_HasUnitToRepair(s16 x, s16 y);

/* Direction bits of the result */
#define DIR_UP 0x01
#define DIR_DOWN 0x02
#define DIR_LEFT 0x04
#define DIR_RIGHT 0x08

/* Bits for the tiles next to (x, y) that hold a unit to repair
 * (Tile_HasUnitToRepair). */
s8 Tile_GetRepairTargetDirections(int x, int y) {
  s8 dirs = 0;

  if (Tile_HasUnitToRepair(x - 1, y) == TRUE) {
    dirs |= DIR_LEFT;
  }
  if (Tile_HasUnitToRepair(x + 1, y) == TRUE) {
    dirs |= DIR_RIGHT;
  }
  if (Tile_HasUnitToRepair(x, y - 1) == TRUE) {
    dirs |= DIR_UP;
  }
  if (Tile_HasUnitToRepair(x, y + 1) == TRUE) {
    dirs |= DIR_DOWN;
  }
  return dirs;
}
