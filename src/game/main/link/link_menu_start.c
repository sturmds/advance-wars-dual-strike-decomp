#include "types.h"
#include "game/task.h"
#include "game/link_menu_state.h"

extern const TaskScriptCmd data_0215ffdc[];

/* Opens the player list screen on the player list view with button 0 selected, and starts its task. */
void LinkMenu_Start(void) {
  data_0215fed0->view = 0;
  data_0215fed0->selected = 0;
  Task_Create(data_0215ffdc, TASK_LIST(3));
}
