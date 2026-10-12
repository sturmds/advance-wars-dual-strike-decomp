#include "types.h"
#include "game/task.h"
#include "game/menu_button.h"

Task* Task_FindByScript(const void* script);
void Sound_PlaySe(u32 soundId);
void ResultStats_DrawPlayerTotals(int player);

extern const TaskScriptCmd data_0215a690[];
extern const MenuButtonTemplate data_0215a560;

/* Left key or button: shows the previous player. The right button comes
 * back below the last player and the left button goes at player 1. */
void ResultStats_PrevPlayer(void) {
  Task* task = Task_FindByScript(data_0215a690);

  if (task->unk_66 <= 1) {
    return;
  }
  task->unk_66--;
  ResultStats_DrawPlayerTotals(task->unk_66);
  if (task->unk_66 == task->unk_6a - 1) {
    MenuButton_Add(0, 0, &data_0215a560, 2);
  }
  if (task->unk_66 == 1) {
    MenuButton_Close(1);
  }
  Sound_PlaySe(0x10002);
}
