#include "types.h"
#include "game/task.h"
#include "game/menu_button.h"

extern const TaskScriptCmd data_0215acc4[];
int Sound_PlaySe(int seId);

/* Plays a sound effect, drops the selectable buttons, turns touch input
 * off and starts the data_0215acc4 script in task list 3. */
void func_02046f3c(void) {
  Sound_PlaySe(0x10006);
  MenuButtons_RemoveSelectable();
  MenuButtons_DisableTouch();
  Task_Create(data_0215acc4, TASK_LIST(3));
}
