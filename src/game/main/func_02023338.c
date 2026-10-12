#include "types.h"

typedef struct Unk_02023338 Unk_02023338;
typedef struct Unk_02023338_1 Unk_02023338_1;

struct Unk_02023338 {
  u8 pad_00[0x8];
  Unk_02023338_1* unk_8;
};

struct Unk_02023338_1 {
  u8 pad_00[0x2];
  s16 unk_2;
};

void Task_JumpToLabel(void*, int);

int func_02023338(Unk_02023338* a0) {
  Task_JumpToLabel(a0, a0->unk_8->unk_2);
  return 1;
}
