#include "types.h"
#include "game/task.h"

typedef struct Unk_0235db04_0 Unk_0235db04_0;

struct Unk_0235db04_0 {
  u8 pad_00[0x52];
  u16 unk_52;
};

extern const TaskScriptCmd data_ov003_02364aac[];

void func_ov003_0235db04(int a0) {
  Unk_0235db04_0* t0 =
      (Unk_0235db04_0*)Task_Create(data_ov003_02364aac, TASK_LIST(2));
  t0->unk_52 = a0;
}
