#include "types.h"
#include "game/menu_state.h"
#include "game/task.h"

extern const TaskScriptCmd data_0215d038[];

void func_020647f8(Task* parent) {
  gMenuState.link.unk_2eb = 1;
  Task_CreateChild(data_0215d038, parent);
}
