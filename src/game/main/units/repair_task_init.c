#include "types.h"
#include "game/Unk_02080440.h"
#include "game/range_map.h"
#include "game/task.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void RangeMap_MarkAdjacentRepairTargets(int x, int y);
s8 Tile_GetRepairTargetDirections(int x, int y);
int NeighborDir_Next(int direction, int mask);
void func_020f78b8(int arg);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Start of the repair command's tile pick, like UnloadTask_Init: marks the
 * adjacent tiles that hold a unit to repair in the range map, stores the
 * direction mask in unk_64 and the first direction in unk_66. */
void RepairTask_Init(Task* task) {
  RangeMap_SetBuffer(STATE->mapTiles->rangeMapTiles);
  RangeMap_Fill(-1);
  RangeMap_MarkAdjacentRepairTargets(STATE->actingUnitDestX,
                                     STATE->actingUnitDestY);
  func_020f78b8(0);
  task->unk_64 = Tile_GetRepairTargetDirections(STATE->actingUnitDestX,
                                                STATE->actingUnitDestY);
  task->unk_66 = 3;
  task->unk_66 = NeighborDir_Next(task->unk_66, task->unk_64);
}
