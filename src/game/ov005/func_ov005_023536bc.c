#include "types.h"
#include "game/task.h"
#include "game/sound.h"

extern const TaskScriptCmd data_ov005_023682e0[];

void func_ov005_023536bc(Task* parent) {
  Task_CreateChild(data_ov005_023682e0, parent);
}
