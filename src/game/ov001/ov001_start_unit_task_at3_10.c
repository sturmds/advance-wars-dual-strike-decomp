#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

void func_020edb84(MapUnit* unit);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Starts the func_020edb84 task for the unit on tile (3, 10), if any: the tile
 * Ov001_SpawnOoziumAt3_10 puts an Oozium on. */
void Ov001_StartUnitTaskAt3_10(void) {
  MapTiles* tiles = STATE->mapTiles;
  MapUnit* unit = &STATE->units[tiles->unk_508[3 + tiles->rowStart[10]]];

  if (unit->type == UNIT_NONE) {
    return;
  }
  func_020edb84(unit);
}
