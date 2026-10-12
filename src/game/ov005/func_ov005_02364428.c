#include "types.h"
#include "game/task.h"

typedef struct Unk_02364428 Unk_02364428;

struct Unk_02364428 {
  u8 pad_00[0x50];
  Task* unk_50;
};

extern const TaskScriptCmd data_ov005_0236aef4[];

Task* func_ov005_02364428(Unk_02364428* a0) {
  Task* t0 = Task_Create(data_ov005_0236aef4, TASK_LIST(3));
  a0->unk_50 = t0;
  return t0;
}
