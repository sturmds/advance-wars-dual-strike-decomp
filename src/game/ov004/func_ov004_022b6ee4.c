#include "types.h"

typedef struct Unk_022b6ee4 Unk_022b6ee4;

struct Unk_022b6ee4 {
  u8 pad_00[0x34];
  int unk_34;
  u8 pad_38[0xc];
  int unk_44;
  u8 pad_48[0xc];
  int unk_54;
  int unk_58;
  int unk_5c;
};

void func_ov004_022b6ee4(Unk_022b6ee4* a0) {
  a0->unk_54 = 0 - 0x4400;
  a0->unk_58 = 0;
  a0->unk_5c = 0x4400;
  a0->unk_44 = 0;
  a0->unk_34 = 0;
}
