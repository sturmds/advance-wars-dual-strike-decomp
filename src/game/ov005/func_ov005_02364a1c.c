#include "types.h"

typedef struct Unk_02364a1c Unk_02364a1c;

struct Unk_02364a1c {
  u8 pad_00[0x4c];
  int unk_4c;
};

void Task_Destroy(int);

void func_ov005_02364a1c(Unk_02364a1c* a0) { Task_Destroy(a0->unk_4c); }
