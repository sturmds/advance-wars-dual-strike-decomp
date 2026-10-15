#include "types.h"
#include "game/Unk_02080440.h"
#include "game/tile_point.h"

int Map_GetDistance(s16 x1, s16 y1, s16 x2, s16 y2);
s32 Rng_Range(s32 min, s32 max);
int Ov006_Abs(int value);

/* Picks random tiles until one holds map cell code `cell` and its distances
 * to a and to b differ by at most `range`; gives up after 101 tries. Returns
 * TRUE with the tile in *outX, *outY. Same shape as Ov006_PickRandomCellNear. */
BOOL Ov006_PickRandomCellBetween(int* outX, int* outY, int cell, TilePoint* a,
                                 TilePoint* b, int range) {
  int tries = 0;
  /* needed to match: the original keeps both lower bounds on the stack */
  int minX = 0;
  int minY = 0;

  for (;;) {
    *outX = Rng_Range(minX, MAP_STATE->unk_1c4->mapWidth - 1);
    *outY = Rng_Range(minY, MAP_STATE->unk_1c4->mapHeight - 1);
    if (cell == MAP_STATE->mapTiles
                    ->mapCells[MAP_STATE->mapTiles->rowStart[*outY] + *outX] &&
        Ov006_Abs(Map_GetDistance(a->x, a->y, *outX, *outY) -
                  Map_GetDistance(b->x, b->y, *outX, *outY)) <= range) {
      break;
    }
    if (tries < 100) {
      tries++;
      continue;
    }
    return FALSE;
  }
  return TRUE;
}
