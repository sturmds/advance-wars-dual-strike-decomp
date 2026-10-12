#include "types.h"
#include "game/task.h"

extern const TaskScriptCmd data_0215fc0c[];
Task* Task_FindByScript(const void* script);
void Sound_PlaySe(u32 soundId);

void func_02075e84(u8* choice) {
  Task* menu = Task_FindByScript(data_0215fc0c);

  menu->unk_52 = *choice;
  Sound_PlaySe(0x10002);
}
