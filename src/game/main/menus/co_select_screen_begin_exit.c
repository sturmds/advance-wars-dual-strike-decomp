#include "types.h"
#include "game/task.h"
#include "game/menu_button.h"

extern const TaskScriptCmd data_0215aa90[];

void CoSelectScreen_BeginExit(void) {
  MenuButtons_RemoveSelectable();
  MenuButtons_DisableTouch();
  Task_Create(data_0215aa90, TASK_LIST(3));
}
