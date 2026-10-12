#include "types.h"
#include "game/task.h"
#include "game/sound.h"

extern const TaskScriptCmd data_ov005_0236afdc[];

void func_ov005_023634e0(Task* parent) {
  Task_CreateChild(data_ov005_0236afdc, parent);
}
