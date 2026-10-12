#include "types.h"
#include "game/task.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern TaskScriptCmd data_02165a84[];

void PopupMenu_PushState(void);
void func_0201e954(void);
void func_020b9e04(void);

/* Remembers menuArg for the battle menu (read back as unk_10 when it opens),
 * saves the open pop-up menu and starts the task of data_02165a84. */
void func_020bde18(int unused, u32 menuArg) {
  data_027c027c->unk_48[data_027c0284]->unk_10 = menuArg;
  PopupMenu_PushState();
  func_0201e954();
  func_020b9e04();
  Task_Create(data_02165a84, TASK_LIST(3));
}
