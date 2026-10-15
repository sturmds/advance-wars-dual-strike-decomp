#include "types.h"
#include "game/task.h"
#include "game/Unk_02080440.h"

extern TaskScriptCmd data_02165a1c[];
void PopupMenu_PushState(void);
void func_0201e954(void);
void func_020b9e04(void);

void func_020bdeb4(int unused, u32 menuArg) {
  data_027c027c->unk_48[data_027c0284]->unk_10 = menuArg;
  PopupMenu_PushState();
  func_0201e954();
  func_020b9e04();
  Task_Create(data_02165a1c, TASK_LIST(3));
}
