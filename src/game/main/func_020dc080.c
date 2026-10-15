#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_target_entry.h"

BOOL Tile_HasUnitToSupply(s16 x, s16 y);
extern UnitTargetEntry* data_022a6dcc;

int func_020dc080(void) {
  UnitTargetEntry* entry = data_022a6dcc;
  int y;
  int x;

  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      if (MAP_STATE->rangeMap[y][x] >= 0 && Tile_HasUnitToSupply(x, y)) {
        entry->unitSlot =
            MAP_STATE->mapTiles->unitAt[x + MAP_STATE->mapTiles->rowStart[y]];
        entry++;
      }
    }
  }
  entry->unitSlot = 0;
  return entry - data_022a6dcc;
}
