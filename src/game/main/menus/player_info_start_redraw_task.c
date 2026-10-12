#include "types.h"
#include "game/task.h"

extern const TaskScriptCmd gPlayerInfoRedrawScript[];

void PlayerInfo_StartRedrawTask(void) {
  Task_Create(gPlayerInfoRedrawScript, TASK_LIST(3));
}
