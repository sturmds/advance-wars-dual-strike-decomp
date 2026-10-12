#include "types.h"

typedef struct Unk_020230bc Unk_020230bc;

struct Unk_020230bc {
  u8 pad_00[0x8];
  int unk_8;
  u8 pad_0c[0x1b];
  u8 unk_27;
};

int func_020230bc(Unk_020230bc* a0) {
  a0->unk_27 = a0->unk_27 | 4;
  a0->unk_8 = a0->unk_8 + 8;
  return 1;
}
