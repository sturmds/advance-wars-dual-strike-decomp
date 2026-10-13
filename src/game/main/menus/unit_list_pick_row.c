#include "types.h"
#include "game/task.h"
#include "game/menu_button.h"
#include "game/unit_list_task.h"

Task* Task_FindByScript(const void* script);
void Sound_PlaySe(u32 soundId);
void Task_ResumeScript(Task* task);

extern const TaskScriptCmd data_0215fad0[];
u8* gUnitListRows;

/* onConfirm of a row button (id = row on screen): stores the list entry of
 * that row in the task, lets the task's script go on and removes the buttons. */
void UnitList_PickRow(MenuButton* button) {
  Task* menu = Task_FindByScript(data_0215fad0);
  UnitListTask* task = (UnitListTask*)menu;

  task->pickedSlot = gUnitListRows[task->topRow + button->id];
  Sound_PlaySe(0x10006);
  Task_ResumeScript(menu);
  MenuButtons_RemoveSelectable();
  MenuButtons_CloseAll();
}
