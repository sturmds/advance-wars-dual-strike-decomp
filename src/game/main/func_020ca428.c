#include "types.h"
#include "game/task.h"

typedef struct Unk_020ca428_0 Unk_020ca428_0;

struct Unk_020ca428_0 {
  u8 pad_00[0x64];
  u16 unk_64;
};

extern const TaskScriptCmd data_02166538[];
BOOL Map_IsCoSelectPhase(void);

int func_020ca428(void) {
  Unk_020ca428_0* t0 =
      (Unk_020ca428_0*)Task_Create(data_02166538, TASK_LIST(3));
  int t1 = Map_IsCoSelectPhase();
  t0->unk_64 = t1;
  return t1;
}
