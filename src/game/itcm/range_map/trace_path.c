#include "types.h"
#include "game/Unk_02080440.h"
#include "game/move_path.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

u32 Rng_Next(void);
void UnitPath_ReverseSteps(s8* start, s8* end);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) (STATE->rangeMap[y][x])

/* Writes to steps the path a unit takes to (x, y), from the costs that
 * RangeMap_FillMoves left in the range map: walks back from (x, y) to the
 * tile of cost 0, each time to the cheapest neighbour (a random one on a
 * tie), then reverses the steps. unk_46 gets the cost of (x, y). Tiles off
 * the map and out of reach (-1) count as 255. */
void RangeMap_TracePath(int x, int y, u8* steps) {
  u8* start = steps;
  s16 best[4]; /* steps from the cheapest neighbours */
  s16 near[4]; /* cost of the tile each step comes from */
  s16 min;
  s16 count;
  int i;
  s16 dir;

  STATE->unk_46 = TILE(x, y);
  dir = 0;
  while (TILE(x, y) != 0) {
    if (x + 1 == STATE->unk_1c4->mapWidth) {
      near[PATH_STEP_LEFT] = 0xff;
    } else {
      near[PATH_STEP_LEFT] = TILE(x + 1, y) & 0xff;
    }
    if (x == 0) {
      near[PATH_STEP_RIGHT] = 0xff;
    } else {
      near[PATH_STEP_RIGHT] = TILE(x - 1, y) & 0xff;
    }
    if (y + 1 == STATE->unk_1c4->mapHeight) {
      near[PATH_STEP_UP] = 0xff;
    } else {
      near[PATH_STEP_UP] = TILE(x, y + 1) & 0xff;
    }
    if (y == 0) {
      near[PATH_STEP_DOWN] = 0xff;
    } else {
      near[PATH_STEP_DOWN] = TILE(x, y - 1) & 0xff;
    }
    count = 0;
    min = 0x100;
    for (i = 0; i < (int)ARRAY_COUNT(near); i++) {
      if (min > near[i]) {
        min = near[i];
      }
    }
    for (i = 0; i < (int)ARRAY_COUNT(near); i++) {
      if (near[i] == min) {
        best[count++] = i;
      }
    }
    switch (count) {
      case 1:
        dir = best[0];
        break;
      case 2:
        dir = best[(Rng_Next() >> 14) & 1];
        break;
      case 3:
        dir = best[Rng_Next() % 3];
        break;
      case 4:
        dir = best[Rng_Next() & 3];
        break;
    }
    *steps++ = dir;
    switch (dir) {
      case PATH_STEP_LEFT:
        x++;
        break;
      case PATH_STEP_RIGHT:
        x--;
        break;
      case PATH_STEP_UP:
        y++;
        break;
      case PATH_STEP_DOWN:
        y--;
        break;
    }
  }
  UnitPath_ReverseSteps((s8*)start, (s8*)steps);
}
