#include "types.h"

typedef struct Unk_0202335c Unk_0202335c;
typedef struct Unk_0202335c_1 Unk_0202335c_1;

struct Unk_0202335c {
  u8 pad_00[0x8];
  Unk_0202335c_1* unk_8;
};

struct Unk_0202335c_1 {
  u8 pad_00[0x4];
  int unk_4;
};

void Task_SetScript(void*, int);

int func_0202335c(Unk_0202335c* a0) {
  Task_SetScript(a0, a0->unk_8->unk_4);
  return 1;
}
