#include "types.h"
#include "game/task.h"

extern TaskScriptCmd data_02166608[];

/* Starts the task that removes a defeated player's units (LinkPlayerDefeatTask_Update runs
 * it). unk_64: player slot, unk_66: reason code. */
void LinkPlayerDefeatTask_Create(u16 player, u16 code) {
  Task* task = Task_Create(data_02166608, TASK_LIST(3));

  task->unk_64 = player;
  task->unk_66 = code;
}
