#include "types.h"

typedef struct Unk_022b2b04 Unk_022b2b04;

struct Unk_022b2b04 {
  u8 pad_00[0x34];
  int unk_34;
  u8 pad_38[0xc];
  int unk_44;
  u8 pad_48[0xc];
  int unk_54;
  int unk_58;
};

void func_ov004_022b2b04(Unk_022b2b04* a0) {
  a0->unk_54 = -64;
  a0->unk_58 = -128;
  a0->unk_44 = 220;
  a0->unk_34 = 0;
}
