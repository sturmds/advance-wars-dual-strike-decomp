#include "types.h"
#include "game/task.h"
#include "game/menu_button.h"

Task* Task_FindByScript(const void* script);
void Sound_PlaySe(u32 soundId);
void ResultStats_DrawPlayerTotals(int player);

extern const TaskScriptCmd data_0215a690[];
extern const MenuButtonTemplate data_0215a5b0;

/* Right key or button: shows the next player (Task.unk_66, counted from 1).
 * The left button appears at player 2 and the right button goes at the last. */
void ResultStats_NextPlayer(void) {
  Task* task = Task_FindByScript(data_0215a690);

  if (task->unk_66 >= task->unk_6a) {
    return;
  }
  task->unk_66++;
  ResultStats_DrawPlayerTotals(task->unk_66);
  if (task->unk_66 == 2) {
    MenuButton_Add(0, 0, &data_0215a5b0, 1);
  }
  if (task->unk_66 == task->unk_6a) {
    MenuButton_Close(2);
  }
  Sound_PlaySe(0x10002);
}
