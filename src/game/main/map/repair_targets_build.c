#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_target_entry.h"

BOOL Tile_HasUnitToRepair(s16 x, s16 y);

/* 8-byte record of the list data_022a6dcc points to. */

extern UnitTargetEntry* data_022a6dcc;

/* Lists the unit slot of every unit to repair on a tile the range map
 * reaches, row by row, ends the list with slot 0 and returns the count. */
int RepairTargets_Build(void) {
  UnitTargetEntry* entry = data_022a6dcc;
  int y;
  int x;

  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      if (MAP_STATE->rangeMap[y][x] >= 0 && Tile_HasUnitToRepair(x, y)) {
        entry->unitSlot =
            MAP_STATE->mapTiles->unitAt[x + MAP_STATE->mapTiles->rowStart[y]];
        entry++;
      }
    }
  }
  entry->unitSlot = 0;
  return entry - data_022a6dcc;
}
