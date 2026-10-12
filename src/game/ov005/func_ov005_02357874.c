#include "types.h"
#include "game/task.h"
#include "game/sound.h"

extern const TaskScriptCmd data_ov005_023694cc[];

void func_ov005_02357874(Task* parent) {
  Task_CreateChild(data_ov005_023694cc, parent);
}
