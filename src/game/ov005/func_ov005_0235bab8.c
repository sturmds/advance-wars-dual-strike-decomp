#include "types.h"
#include "game/task.h"
#include "game/sound.h"

extern const TaskScriptCmd data_ov005_0236a578[];

void func_ov005_0235bab8(Task* parent) {
  Task_CreateChild(data_ov005_0236a578, parent);
}
