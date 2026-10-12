#include "types.h"
#include "game/task.h"
#include "game/sound.h"

extern const TaskScriptCmd data_ov005_02368af8[];

void func_ov005_02355268(Task* parent) {
  Task_CreateChild(data_ov005_02368af8, parent);
}
