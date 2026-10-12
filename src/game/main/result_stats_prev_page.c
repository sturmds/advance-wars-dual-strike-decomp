#include "types.h"
#include "game/task.h"
#include "game/menu_button.h"

Task* Task_FindByScript(const void* script);
void Sound_PlaySe(u32 soundId);
void ResultStats_PrintFirstPageLabel(void);

extern const TaskScriptCmd data_0215a690[];
extern const MenuButtonTemplate data_0215a600;

/* Up key or button: back to the first page of unit counts (Task.unk_68
 * 1 to 0). Swaps the up button for the down button. */
void ResultStats_PrevPage(void) {
  Task* task = Task_FindByScript(data_0215a690);

  if (task->unk_68 == 0) {
    return;
  }
  task->unk_68--;
  ResultStats_PrintFirstPageLabel();
  MenuButton_Add(0, 0, &data_0215a600, 4);
  MenuButton_Close(3);
  Sound_PlaySe(0x10002);
}
