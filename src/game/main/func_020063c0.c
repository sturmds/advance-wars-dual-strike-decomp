#include "types.h"
#include "game/task.h"

typedef struct Unk_020063c0_0 Unk_020063c0_0;

struct Unk_020063c0_0 {
  u8 pad_00[0x64];
  u16 unk_64;
};

extern const TaskScriptCmd data_02157244[];

void func_020063c0(int a0) {
  Unk_020063c0_0* t0 =
      (Unk_020063c0_0*)Task_Create(data_02157244, TASK_LIST(3));
  if ((int)t0 != 0) {
    t0->unk_64 = a0;
  }
}
