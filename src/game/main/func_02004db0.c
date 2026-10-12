#include "types.h"
#include "game/task.h"

typedef struct Unk_02004db0_0 Unk_02004db0_0;

struct Unk_02004db0_0 {
  u8 pad_00[0x64];
  u16 unk_64;
};

extern const TaskScriptCmd data_021572e4[];

void func_02004db0(int a0, Task* parent) {
  Unk_02004db0_0* t0 = (Unk_02004db0_0*)Task_CreateChild(data_021572e4, parent);
  if ((int)t0 != 0) {
    t0->unk_64 = a0;
  }
}
