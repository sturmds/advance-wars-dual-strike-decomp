#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

/* TRUE when a tile with a nonzero MapTiles.unk_2338 byte holds an Oozium. The name is a guess; no caller found. */
BOOL Map_HasOoziumOnMarkedTile(void) {
  int x;
  int y;

  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      if (MAP_STATE->mapTiles->unk_2338[MAP_TILE_INDEX(x, y)] != 0 &&
          MAP_STATE->units[MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(x, y)]]
                  .type == UNIT_OOZIUM) {
        return TRUE;
      }
    }
  }
  return FALSE;
}
