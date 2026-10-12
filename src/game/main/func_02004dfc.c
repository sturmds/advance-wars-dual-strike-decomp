#include "types.h"
#include "game/task.h"

typedef struct Unk_02004dfc_0 Unk_02004dfc_0;

struct Unk_02004dfc_0 {
  u8 pad_00[0x64];
  u16 unk_64;
};

extern const TaskScriptCmd data_021572e4[];

void func_02004dfc(int a0) {
  Unk_02004dfc_0* t0 =
      (Unk_02004dfc_0*)Task_Create(data_021572e4, TASK_LIST(3));
  if ((int)t0 != 0) {
    t0->unk_64 = a0;
  }
}
