#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_target_entry.h"

BOOL Tile_HasUnitToSupply(s16 x, s16 y);
extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern UnitTargetEntry* data_022a6dcc;
#define STATE (data_027c027c->unk_48[data_027c0284])

int func_020dc080(void) {
  UnitTargetEntry* entry = data_022a6dcc;
  int y;
  int x;

  for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
      if (STATE->rangeMap[y][x] >= 0 && Tile_HasUnitToSupply(x, y)) {
        entry->unitSlot =
            STATE->mapTiles->unitAt[x + STATE->mapTiles->rowStart[y]];
        entry++;
      }
    }
  }
  entry->unitSlot = 0;
  return entry - data_022a6dcc;
}
