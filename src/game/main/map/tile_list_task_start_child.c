#include "types.h"
#include "game/task.h"
#include "game/Unk_02168140.h"

extern const TaskScriptCmd data_02168140[];

/* Starts the data_02168140 task under parent with a tile and a list of tile
 * positions (see TileListTask_MoveToNext). Name is a guess. */
void TileListTask_StartChild(int tileX, int tileY, void* list, Task* parent) {
  Unk_02168140Task* task =
      (Unk_02168140Task*)Task_CreateChild(data_02168140, parent);

  task->unk_2c = tileX;
  task->unk_30 = tileY;
  task->unk_4c = list;
}
