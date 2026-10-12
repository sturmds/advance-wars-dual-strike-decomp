#include "types.h"

typedef struct Unk_02081214 Unk_02081214;

struct Unk_02081214 {
  u8 pad_00[0x30];
  int unk_30;
};

void Mem_Free(int);

void func_02081214(Unk_02081214* a0) { Mem_Free(a0->unk_30); }
