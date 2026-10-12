#include "types.h"
#include "game/task.h"

typedef struct Unk_02004a1c_0 {
  u8 pad_00[0x64];
  u16 unk_64;
} Unk_02004a1c_0;
void ScreenEffect_SetBrightenMode(void);
extern const TaskScriptCmd data_02157344[];

void func_02004ae4(int value, Task* parent) {
  Unk_02004a1c_0* task =
      (Unk_02004a1c_0*)Task_CreateChild(data_02157344, parent);

  if (task != NULL) {
    task->unk_64 = value;
    ScreenEffect_SetBrightenMode();
  }
}
