#include "types.h"
#include "game/task.h"
#include "game/Unk_02080440.h"
#include "game/menu_button.h"

extern const TaskScriptCmd data_0215fb68[];
Task* Task_FindByScript(const void* script);
void Sound_PlaySe(u32 soundId);
void Task_ResumeScript(Task* task);

void func_020754d0(void) {
  Task* menu = Task_FindByScript(data_0215fb68);

  Sound_PlaySe(0x10009);
  Task_ResumeScript(menu);
  MenuButtons_RemoveSelectable();
  MenuButtons_CloseAll();
  *data_027c027c->unk_40 = 1;
}
