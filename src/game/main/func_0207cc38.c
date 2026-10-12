#include "types.h"
#include "game/task.h"

typedef struct Unk_0207cc38 Unk_0207cc38;

struct Unk_0207cc38 {
  u8 pad_00[0x4c];
  u16 unk_4c;
  u8 pad_4e[0xa];
  int unk_58;
};

extern const TaskScriptCmd data_0215fed4[];

void func_0207cc38(Unk_0207cc38* a0) {
  a0->unk_4c = 0;
  a0->unk_58 = 0;
  Task_Create(data_0215fed4, a0);
}
