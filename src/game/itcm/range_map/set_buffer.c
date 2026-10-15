#include "types.h"
#include "game/Unk_02080440.h"
#include "game/range_map.h"

extern MoveSearch* data_027c0758;

/* Points each rangeMap row at its row of tileMap, then stores the map size
 * in MoveSearch fields x and y (header comments call them the tile being
 * expanded; this code writes width and height there). */
void RangeMap_SetBuffer(void* tileMap) {
  int row;

  for (row = 0; row < MAP_STATE->unk_1c4->mapHeight; row++) {
    MAP_STATE->rangeMap[row] =
        (s8*)tileMap + MAP_STATE->mapTiles->rowStart[row];
  }
  data_027c0758->x = MAP_STATE->unk_1c4->mapWidth;
  data_027c0758->y = MAP_STATE->unk_1c4->mapHeight;
}
