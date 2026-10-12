#include "types.h"
#include "game/covering_fire_task.h"
#include "game/task.h"
#include "game/Unk_0216928c.h"

extern const TaskScriptCmd data_0216928c[];

/* Script step of gCoveringFireScript: starts a data_0216928c child task at
 * the missile's tile. */
void CoveringFire_StartBlast(CoveringFireTask* task) {
  Unk_0216928c* child =
      (Unk_0216928c*)Task_CreateChild(data_0216928c, (Task*)task);

  child->unk_2c = task->targetX;
  child->unk_30 = task->targetY;
  child->unk_4a = 30;
  child->unk_54 = 0;
}
