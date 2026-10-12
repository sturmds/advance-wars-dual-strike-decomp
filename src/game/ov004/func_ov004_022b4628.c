#include "types.h"

typedef struct Unk_022b4628 Unk_022b4628;

struct Unk_022b4628 {
  u8 pad_00[0x34];
  int unk_34;
  u8 pad_38[0x1c];
  int unk_54;
  int unk_58;
  int unk_5c;
};

void func_ov004_022b4628(Unk_022b4628* a0) {
  a0->unk_54 = 0x4400;
  a0->unk_58 = 0;
  a0->unk_5c = 0 - 0x4400;
  a0->unk_34 = 0;
}
