#include "types.h"
#include "game/task.h"
#include "game/sound.h"

extern const TaskScriptCmd data_ov005_0236b38c[];

void func_ov005_02365ea8(Task* parent) {
  Task_CreateChild(data_ov005_0236b38c, parent);
}
