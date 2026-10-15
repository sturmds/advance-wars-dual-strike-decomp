#include "types.h"
#include "game/task.h"
#include "game/Unk_02080440.h"
#include "game/menu_button.h"

extern const TaskScriptCmd data_0215fad0[];

Task* Task_FindByScript(const void* script);
void Sound_PlaySe(u32 soundId);
void Task_ResumeScript(Task* task);

/* Closes the menu of task data_0215fad0: plays a sound, lets the
 * task's script go on, removes the menu buttons and sets the u16 that
 * unk_40 points to to 1. */
void UnitList_Close(void) {
  Task* menu = Task_FindByScript(data_0215fad0);

  Sound_PlaySe(0x10009);
  Task_ResumeScript(menu);
  MenuButtons_RemoveSelectable();
  MenuButtons_CloseAll();
  *data_027c027c->unk_40 = 1;
}
