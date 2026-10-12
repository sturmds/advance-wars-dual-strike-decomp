#include "types.h"
#include "game/Unk_02080440.h"

BOOL Tile_HasUnitToRepair(s16 x, s16 y);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* 8-byte record of the list data_022a6dcc points to. */
typedef struct UnitTargetEntry {
  u16 unitSlot;
  u8 pad_02[6];
} UnitTargetEntry;

extern UnitTargetEntry* data_022a6dcc;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Lists the unit slot of every unit to repair on a tile the range map
 * reaches, row by row, ends the list with slot 0 and returns the count. */
int RepairTargets_Build(void) {
  UnitTargetEntry* entry = data_022a6dcc;
  int y;
  int x;

  for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
      if (STATE->rangeMap[y][x] >= 0 && Tile_HasUnitToRepair(x, y)) {
        entry->unitSlot =
            STATE->mapTiles->unitAt[x + STATE->mapTiles->rowStart[y]];
        entry++;
      }
    }
  }
  entry->unitSlot = 0;
  return entry - data_022a6dcc;
}
