#include "types.h"

typedef struct Unk_0235710c Unk_0235710c;

struct Unk_0235710c {
  u8 pad_00[0x30];
  int unk_30;
  int unk_34;
  int unk_38;
  u8 pad_3c[0x16];
  u16 unk_52;
};

void func_ov002_0235710c(Unk_0235710c* a0) {
  a0->unk_52 = 0;
  a0->unk_38 = a0->unk_30;
  a0->unk_34 = 0;
}
