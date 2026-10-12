#include "types.h"

typedef struct Unk_0210928c Unk_0210928c;

struct Unk_0210928c {
  u8 pad_00[0x8];
  int unk_8;
  int unk_c;
};

int func_0210928c(Unk_0210928c* a0) { return (a0->unk_8 >= a0->unk_c) ? 1 : 0; }
