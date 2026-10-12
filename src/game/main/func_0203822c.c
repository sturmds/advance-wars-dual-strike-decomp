#include "types.h"
#include "game/Unk_0229c734.h"

typedef struct Unk_020381b8_Task {
  u8 pad_00[0x3a];
  s16 unk_3a;
} Unk_020381b8_Task;
extern u32 data_0215a44c[];
Unk_020381b8_Task* Task_FindByScript(void* script);
void Task_ResumeScript(Unk_020381b8_Task* task);

void func_0203822c(int value) {
  Unk_020381b8_Task* task = Task_FindByScript(data_0215a44c);

  data_0229c734->unk_a4 = 1;
  task->unk_3a = value;
  Task_ResumeScript(task);
}
