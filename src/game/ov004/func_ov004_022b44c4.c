#include "types.h"

typedef struct Unk_022b44c4 Unk_022b44c4;

struct Unk_022b44c4 {
  u8 pad_00[0x34];
  int unk_34;
  u8 pad_38[0x1c];
  int unk_54;
  int unk_58;
};

void func_ov004_022b44c4(Unk_022b44c4* a0) {
  a0->unk_54 = -96;
  a0->unk_58 = 208;
  a0->unk_34 = 0;
}
