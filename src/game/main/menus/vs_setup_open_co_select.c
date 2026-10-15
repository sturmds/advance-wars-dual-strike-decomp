#include "types.h"
#include "game/task.h"
#include "game/menu_button.h"
#include "game/menu_state.h"

extern const TaskScriptCmd data_0215ce68[];

/* Puts the setup cursor (ids 8-15 are COs) on setup.selectedSlot's front CO and
 * starts a task. The task's purpose is a guess. */
void* VsSetup_OpenCoSelect(void) {
  MenuButtons_RemoveSelectable();
  MenuButtons_DisableTouch();
  gMenuState.setup.cursorButton = gMenuState.setup.selectedSlot * 2 + 8;
  return Task_Create(data_0215ce68, TASK_LIST(3));
}
