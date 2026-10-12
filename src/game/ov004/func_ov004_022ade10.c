#include "types.h"
#include "game/task.h"
#include "game/sound.h"

extern const TaskScriptCmd data_ov004_022be9f8[];

void func_ov004_022ade10(Task* parent) {
  Task_CreateChild(data_ov004_022be9f8, parent);
}
