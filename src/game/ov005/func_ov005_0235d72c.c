#include "types.h"
#include "game/task.h"
#include "game/sound.h"

extern const TaskScriptCmd data_ov005_0236a724[];

void func_ov005_0235d72c(Task* parent) {
  Task_CreateChild(data_ov005_0236a724, parent);
}
