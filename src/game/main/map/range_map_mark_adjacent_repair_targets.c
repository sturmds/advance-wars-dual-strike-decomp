#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* Defined with s16 parameters; declared with int here because the original
 * passes x - 1 and the like without narrowing them. */
BOOL Tile_HasUnitToRepair(int x, int y);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Marks each of the 4 tiles next to (x, y) that holds a unit to repair (or
 * supply) as reachable (0) in the range map. */
void RangeMap_MarkAdjacentRepairTargets(int x, int y) {
  if (Tile_HasUnitToRepair(x, y - 1)) {
    STATE->rangeMap[y - 1][x] = 0;
  }
  if (Tile_HasUnitToRepair(x - 1, y)) {
    STATE->rangeMap[y][x - 1] = 0;
  }
  if (Tile_HasUnitToRepair(x, y + 1)) {
    STATE->rangeMap[y + 1][x] = 0;
  }
  if (Tile_HasUnitToRepair(x + 1, y)) {
    STATE->rangeMap[y][x + 1] = 0;
  }
}
