#include "types.h"
#include "game/task.h"
#include "game/menu_button.h"

extern const TaskScriptCmd data_0215fc44[];
int Sound_PlaySe(int seId);

/* Plays a sound effect, closes all menu buttons, drops the selectable ones
 * and starts the data_0215fc44 script in task list 3. */
void func_02075f6c(void) {
  Sound_PlaySe(0x10006);
  MenuButtons_CloseAll();
  MenuButtons_RemoveSelectable();
  Task_Create(data_0215fc44, TASK_LIST(3));
}
