#include "types.h"
#include "game/task.h"
#include "game/menu_button.h"

Task* Task_FindByScript(const void* script);
void Sound_PlaySe(u32 soundId);
void ResultStats_PrintSecondPageLabel(void);

extern const TaskScriptCmd data_0215a690[];
extern const MenuButtonTemplate data_0215a5d8;

/* Down key or button: shows the second page of unit counts (Task.unk_68
 * 0 to 1). Swaps the down button for the up button. */
void ResultStats_NextPage(void) {
  Task* task = Task_FindByScript(data_0215a690);

  if (task->unk_68 == 1) {
    return;
  }
  task->unk_68++;
  ResultStats_PrintSecondPageLabel();
  MenuButton_Add(0, 0, &data_0215a5d8, 3);
  MenuButton_Close(4);
  Sound_PlaySe(0x10002);
}
