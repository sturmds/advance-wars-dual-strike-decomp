#include "types.h"
#include "game/task.h"
#include "game/sound.h"

extern const TaskScriptCmd data_ov005_02369634[];

void func_ov005_02358970(Task* parent) {
  Task_CreateChild(data_ov005_02369634, parent);
}
