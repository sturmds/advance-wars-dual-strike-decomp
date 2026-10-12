#include "types.h"

typedef struct Unk_02027aa0 Unk_02027aa0;

struct Unk_02027aa0 {
  u8 pad_00[0x48];
  int unk_48;
};

void func_02027aa0(Unk_02027aa0* a0, int a1) { a0->unk_48 = a1; }
