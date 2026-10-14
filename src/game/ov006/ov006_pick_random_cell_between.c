#include "types.h"
#include "game/Unk_02080440.h"
#include "game/tile_point.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int Map_GetDistance(s16 x1, s16 y1, s16 x2, s16 y2);
s32 Rng_Range(s32 min, s32 max);
int Ov006_Abs(int value);

#define STATE (data_027c027c->unk_48[data_027c0284])

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
    *outX = Rng_Range(minX, STATE->unk_1c4->mapWidth - 1);
    *outY = Rng_Range(minY, STATE->unk_1c4->mapHeight - 1);
    if (cell == STATE->mapTiles
                    ->mapCells[STATE->mapTiles->rowStart[*outY] + *outX] &&
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
