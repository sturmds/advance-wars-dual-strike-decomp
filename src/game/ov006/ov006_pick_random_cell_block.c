#include "types.h"
#include "game/Unk_02080440.h"
#include "game/tile_point.h"

extern Unk_02080440* data_027c027c;
extern u32 data_027c0284;

int Map_GetDistance(s16 x1, s16 y1, s16 x2, s16 y2);
s32 Rng_Range(s32 min, s32 max);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Picks a random tile off the map edge whose 3x3 block holds only map cell
 * code `cell` and, when avoid is set, that lies at least 15 tiles from it.
 * Edge and distance misses do not count; a block miss counts as one of
 * 101 tries. Returns TRUE with the centre in *outX, *outY. */
BOOL Ov006_PickRandomCellBlock(int* outX, int* outY, int cell,
                               TilePoint* avoid) {
  int tries = 0;
  /* needed to match: the original keeps both lower bounds on the stack */
  int minX = 0;
  int minY = 0;
  int x;
  int y;

  for (;;) {
  pick:
    *outX = Rng_Range(minX, STATE->unk_1c4->mapWidth - 1);
    *outY = Rng_Range(minY, STATE->unk_1c4->mapHeight - 1);
    if (*outX == 0 || *outX == STATE->unk_1c4->mapWidth - 1 || *outY == 0 ||
        *outY == STATE->unk_1c4->mapHeight - 1) {
      continue;
    }
    if (avoid != NULL &&
        Map_GetDistance(avoid->x, avoid->y, *outX, *outY) < 15) {
      continue;
    }
    for (y = *outY - 1; y <= *outY + 1; y++) {
      for (x = *outX - 1; x <= *outX + 1; x++) {
        if (cell !=
            STATE->mapTiles->mapCells[STATE->mapTiles->rowStart[y] + x]) {
          if (tries < 100) {
            tries++;
            goto pick;
          }
          return FALSE;
        }
      }
    }
    return TRUE;
  }
}
