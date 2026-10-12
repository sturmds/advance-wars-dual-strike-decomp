#include "types.h"
#include "game/task.h"

typedef struct Unk_02364a2c Unk_02364a2c;

struct Unk_02364a2c {
  u8 pad_00[0x4c];
  Task* unk_4c;
};

extern const TaskScriptCmd data_ov005_0236aec4[];

Task* func_ov005_02364a2c(Unk_02364a2c* a0) {
  Task* t0 = Task_Create(data_ov005_0236aec4, TASK_LIST(3));
  a0->unk_4c = t0;
  return t0;
}
