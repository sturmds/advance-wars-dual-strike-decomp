#include "types.h"
#include "game/unit_show_task.h"

UnitShowTask* UnitShowTask_Spawn(int x, int y, int player, int type);

/* Makes a show task for a unit and gives it a path to walk (16 move steps,
 * as UnitShowTask_SetPath reads them). NULL when no task could be made. */
UnitShowTask* UnitShowTask_CreateMoving(int x, int y, int player, int type,
                                        u8* path) {
  UnitShowTask* task = UnitShowTask_Spawn(x, y, player, type);

  if (task == NULL) {
    return NULL;
  }
  UnitShowTask_SetPath(task, path);
  return task;
}
