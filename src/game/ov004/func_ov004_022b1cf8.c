#include "types.h"

typedef struct Unk_022b1cf8 Unk_022b1cf8;

struct Unk_022b1cf8 {
  u8 pad_00[0x34];
  int unk_34;
  u8 pad_38[0xc];
  int unk_44;
  int unk_48;
  int unk_4c;
};

void func_ov004_022b1cf8(Unk_022b1cf8* a0) {
  a0->unk_44 = 20;
  a0->unk_48 = 15;
  a0->unk_4c = 15;
  a0->unk_34 = 0;
}
