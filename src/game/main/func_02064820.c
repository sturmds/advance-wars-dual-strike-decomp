#include "types.h"
#include "game/menu_state.h"
#include "game/task.h"

extern const TaskScriptCmd data_0215d1d8[];

void func_02064820(Task* parent) {
  gMenuState.link.unk_2eb = 0;
  Task_CreateChild(data_0215d1d8, parent);
}
