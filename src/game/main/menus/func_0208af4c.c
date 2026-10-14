#include "types.h"
#include "game/menu_button.h"
#include "game/task.h"

void Sound_PlaySe(u32 soundId);
void Task_ResumeByScript(void* script);
extern TaskScriptCmd data_021616c4[];

void func_0208af4c(void) {
  Sound_PlaySe(0x10006);
  Task_ResumeByScript(data_021616c4);
  MenuButton_Close(254);
}
