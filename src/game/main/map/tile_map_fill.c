#include "types.h"
#include "game/Unk_02080440.h"
#include "game/range_map.h"

/* Sets every tile of a per-tile map to value, without touching rangeMap.
 * Afterwards rangeMap points at MapTiles.rangeMapTiles again.
 * value has to be u8 to match; game/range_map.h still declares it int, so
 * this file does not include that header. */
void TileMap_Fill(void* tileMap, u8 value) {
  u8* rows[40];
  int row;
  int x;
  int y;

  /* Needed to match: a separate counter for this loop (with y alone, the
     * second loop gets other registers). */
  for (row = 0; row < MAP_STATE->unk_1c4->mapHeight; row++) {
    rows[row] = (u8*)tileMap + MAP_STATE->mapTiles->rowStart[row];
  }
  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      rows[y][x] = value;
    }
  }
  RangeMap_SetBuffer(MAP_STATE->mapTiles->rangeMapTiles);
}
