#include "types.h"
#include "game/task.h"

typedef struct Unk_020ce158_0 Unk_020ce158_0;

struct Unk_020ce158_0 {
  u8 pad_00[0x68];
  u16 unk_68;
};

extern const TaskScriptCmd data_021666b0[];

void func_020ce158(int a0) {
  Unk_020ce158_0* t0 =
      (Unk_020ce158_0*)Task_Create(data_021666b0, TASK_LIST(3));
  t0->unk_68 = a0;
}
