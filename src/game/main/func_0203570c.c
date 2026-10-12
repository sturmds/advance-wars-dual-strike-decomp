#include "types.h"
#include "game/task.h"

typedef struct Unk_0203570c Unk_0203570c;

struct Unk_0203570c {
  u8 pad_00[0x4c];
  u16 unk_4c;
};

extern const TaskScriptCmd data_02159da8[];

void func_0203570c(Unk_0203570c* a0) {
  a0->unk_4c = 0;
  Task_Create(data_02159da8, a0);
}
