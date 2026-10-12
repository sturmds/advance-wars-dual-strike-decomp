#include "types.h"
#include "game/task.h"

typedef struct Unk_02088528_0 Unk_02088528_0;

struct Unk_02088528_0 {
  u8 pad_00[0x52];
  u16 unk_52;
};

extern const TaskScriptCmd data_02160b84[];

void func_02088528(int a0) {
  Unk_02088528_0* t0 =
      (Unk_02088528_0*)Task_Create(data_02160b84, TASK_LIST(3));
  t0->unk_52 = a0;
}
