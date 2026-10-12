#include "types.h"

typedef struct Unk_022b01dc Unk_022b01dc;

struct Unk_022b01dc {
  u8 pad_00[0x34];
  int unk_34;
  u8 pad_38[0xc];
  int unk_44;
  u8 pad_48[0xc];
  int unk_54;
  int unk_58;
};

void func_ov004_022b01dc(Unk_022b01dc* a0) {
  a0->unk_54 = 0;
  a0->unk_58 = 8;
  a0->unk_44 = 18;
  a0->unk_34 = 0;
}
