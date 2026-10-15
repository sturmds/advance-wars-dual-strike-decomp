#include "types.h"
#include "game/Unk_02080440.h"
#include "game/range_spread.h"
#include "game/task.h"

void* Mem_Alloc(u32 size);

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
  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      if (MAP_STATE->rangeMap[y][x] >= 0) {
        dots[RANGE_SPREAD_COUNT(task)].x = MAP_STATE->actingUnitDestX << 4;
        dots[RANGE_SPREAD_COUNT(task)].y = MAP_STATE->actingUnitDestY << 4;
        dots[RANGE_SPREAD_COUNT(task)].vx =
            (x - MAP_STATE->actingUnitDestX) * 2;
        dots[RANGE_SPREAD_COUNT(task)].vy =
            (y - MAP_STATE->actingUnitDestY) * 2;
        RANGE_SPREAD_COUNT(task)++;
      }
    }
  }
}
