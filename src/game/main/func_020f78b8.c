#include "types.h"
#include "game/task.h"

typedef struct Unk_020f78b8_0 Unk_020f78b8_0;

struct Unk_020f78b8_0 {
  u8 pad_00[0x64];
  u16 unk_64;
};

extern const TaskScriptCmd data_02169dac[];

void func_020f78b8(int a0) {
  Unk_020f78b8_0* t0 =
      (Unk_020f78b8_0*)Task_Create(data_02169dac, TASK_LIST(3));
  t0->unk_64 = a0;
}
