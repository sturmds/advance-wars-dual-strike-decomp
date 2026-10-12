#include "types.h"

typedef struct Unk_020fabfc Unk_020fabfc;

struct Unk_020fabfc {
  u8 pad_00[0x44];
  u16 unk_44;
  u8 pad_46[0xe];
  int unk_54;
  u8 pad_58[0xc];
  u16 unk_64;
  u16 unk_66;
};

void func_020fabfc(Unk_020fabfc* a0) {
  a0->unk_64 = 31;
  a0->unk_66 = 15;
  a0->unk_54 = 32;
  a0->unk_44 = 0;
}
