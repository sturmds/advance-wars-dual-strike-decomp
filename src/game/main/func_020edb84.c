#include "types.h"
#include "game/task.h"

typedef struct Unk_020edb84_0 Unk_020edb84_0;

struct Unk_020edb84_0 {
  u8 pad_00[0x4c];
  int unk_4c;
};

extern const TaskScriptCmd data_02168f48[];

void func_020edb84(int a0) {
  Unk_020edb84_0* t0 =
      (Unk_020edb84_0*)Task_Create(data_02168f48, TASK_LIST(3));
  t0->unk_4c = a0;
}
