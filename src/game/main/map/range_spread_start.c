#include "types.h"
#include "game/Unk_02080440.h"
#include "game/range_spread.h"
#include "game/task.h"

void* Mem_Alloc(u32 size);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Start of the range spread task: one dot for each tile with a rangeMap
 * value of 0 or more, starting on the acting unit's destination. */
void RangeSpread_Start(Task* task) {
  RangeSpreadDot* dots;
  int x;
  int y;

  task->unk_4c = Mem_Alloc(RANGE_SPREAD_MAX_DOTS * sizeof(RangeSpreadDot));
  dots = task->unk_4c;
  RANGE_SPREAD_COUNT(task) = 0;
  task->unk_66 = 0;
  for (y = 0; y < GAME->unk_1c4->mapHeight; y++) {
    for (x = 0; x < GAME->unk_1c4->mapWidth; x++) {
      if (GAME->rangeMap[y][x] >= 0) {
        dots[RANGE_SPREAD_COUNT(task)].x = GAME->actingUnitDestX << 4;
        dots[RANGE_SPREAD_COUNT(task)].y = GAME->actingUnitDestY << 4;
        dots[RANGE_SPREAD_COUNT(task)].vx = (x - GAME->actingUnitDestX) * 2;
        dots[RANGE_SPREAD_COUNT(task)].vy = (y - GAME->actingUnitDestY) * 2;
        RANGE_SPREAD_COUNT(task)++;
      }
    }
  }
}
