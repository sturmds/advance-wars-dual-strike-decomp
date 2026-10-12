#include "types.h"
#include "game/task.h"

typedef struct Unk_02004cf4_0 Unk_02004cf4_0;

struct Unk_02004cf4_0 {
  u8 pad_00[0x64];
  u16 unk_64;
};

extern const TaskScriptCmd data_02157224[];

void func_02004cf4(int a0, Task* parent) {
  Unk_02004cf4_0* t0 = (Unk_02004cf4_0*)Task_CreateChild(data_02157224, parent);
  if ((int)t0 != 0) {
    t0->unk_64 = a0;
  }
}
