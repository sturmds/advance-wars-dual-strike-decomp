#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE_INDEX(x, y) ((x) + STATE->mapTiles->rowStart[y])

/* TRUE when a tile with a nonzero MapTiles.unk_2338 byte holds an Oozium. The name is a guess; no caller found. */
BOOL Map_HasOoziumOnMarkedTile(void) {
  int x;
  int y;

  for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
      if (STATE->mapTiles->unk_2338[TILE_INDEX(x, y)] != 0 &&
          STATE->units[STATE->mapTiles->unitAt[TILE_INDEX(x, y)]].type ==
              UNIT_OOZIUM) {
        return TRUE;
      }
    }
  }
  return FALSE;
}
