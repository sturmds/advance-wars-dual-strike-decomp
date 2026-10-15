#include "types.h"
#include "game/Unk_02080440.h"
#include "game/task.h"

void PopupMenu_DestroyCommon(Task* task);
void PopupMenu_ClearBox(int unused0, int unused1, int unused2, int unused3);
void func_020f79a4(void);
void MenuDepth_Pop(void);

/* onDestroy of a style 1 pop-up menu: clears layer 1, runs the game state's
 * second hook and closes one menu level. */
void PopupMenu_DestroyStyle1(Task* task) {
  PopupMenu_DestroyCommon(task);
  PopupMenu_ClearBox(0, 1, 6, 12);
  if (data_027c027c->unk_50[data_027c0284]->second != NULL) {
    data_027c027c->unk_50[data_027c0284]->second();
  }
  func_020f79a4();
  MenuDepth_Pop();
}
