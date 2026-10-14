#include "types.h"
#include "game/Unk_02080440.h"

s32 Rng_Range(s32 min, s32 max);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Picks random tiles, up to 101 times, until one has cell code cell. Returns 1 with
 * the tile in *x and *y, or 0. */
int Ov006_FindRandomCell(int* x, int* y, int cell) {
  int tries = 0;

  for (;;) {
    *x = Rng_Range(0, STATE->unk_1c4->mapWidth - 1);
    *y = Rng_Range(0, STATE->unk_1c4->mapHeight - 1);
    if (cell == STATE->mapTiles->mapCells[*x + STATE->mapTiles->rowStart[*y]]) {
      break;
    }
    if (tries < 100) {
      tries++;
      continue;
    }
    return 0;
  }
  return 1;
}
