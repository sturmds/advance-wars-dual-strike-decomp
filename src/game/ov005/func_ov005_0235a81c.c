#include "types.h"

typedef struct Unk_0235a81c Unk_0235a81c;

struct Unk_0235a81c {
  u8 pad_00[0x64];
  int unk_64;
};

void Task_Destroy(int);

void func_ov005_0235a81c(Unk_0235a81c* a0) { Task_Destroy(a0->unk_64); }
