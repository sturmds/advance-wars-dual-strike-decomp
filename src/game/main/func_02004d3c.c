#include "types.h"
#include "game/task.h"

typedef struct Unk_02004d3c_0 Unk_02004d3c_0;

struct Unk_02004d3c_0 {
  u8 pad_00[0x64];
  u16 unk_64;
};

extern const TaskScriptCmd data_02157224[];

void func_02004d3c(int a0) {
  Unk_02004d3c_0* t0 =
      (Unk_02004d3c_0*)Task_Create(data_02157224, TASK_LIST(3));
  if ((int)t0 != 0) {
    t0->unk_64 = a0;
  }
}
