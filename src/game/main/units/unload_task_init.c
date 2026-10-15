#include "types.h"
#include "game/Unk_02080440.h"
#include "game/range_map.h"
#include "game/task.h"

extern u32 gLinkPlay;

void RangeMap_ClearAdjacent(int x, int y, int dirs);
s8 Unit_GetDropDirections(int x, int y, int slot);
int NeighborDir_Next(int direction, int mask);
void func_020f78b8(int arg);
void func_020802e8(void);

/* Start of the unload command's tile pick: unk_68 holds the cargo slot. Marks
 * the free drop tiles in the range map, stores the free-direction mask in
 * unk_64 and the first free direction in unk_66. */
void UnloadTask_Init(Task* task) {
  int dirs;

  dirs = Unit_GetDropDirections(MAP_STATE->actingUnitDestX,
                                MAP_STATE->actingUnitDestY,
                                MAP_STATE->actingUnit->cargo[task->unk_68]);
  RangeMap_SetBuffer(MAP_STATE->mapTiles->rangeMapTiles);
  RangeMap_Fill(-1);
  RangeMap_ClearAdjacent(MAP_STATE->actingUnitDestX, MAP_STATE->actingUnitDestY,
                         dirs);
  func_020f78b8(0);
  task->unk_64 = dirs;
  task->unk_66 = 3;
  task->unk_6a = 0;
  if (gLinkPlay != 0) {
    func_020802e8();
  }
  task->unk_66 = NeighborDir_Next(task->unk_66, dirs);
}
