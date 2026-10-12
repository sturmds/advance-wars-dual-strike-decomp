#include "types.h"
#include "game/task.h"

typedef struct Unk_020063e8_0 Unk_020063e8_0;

struct Unk_020063e8_0 {
  u8 pad_00[0x64];
  u16 unk_64;
};

extern const TaskScriptCmd data_02157264[];

void func_020063e8(int a0, Task* parent) {
  Unk_020063e8_0* t0 = (Unk_020063e8_0*)Task_CreateChild(data_02157264, parent);
  if ((int)t0 != 0) {
    t0->unk_64 = a0;
  }
}
