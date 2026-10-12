#include "types.h"

typedef struct Unk_022b473c Unk_022b473c;

struct Unk_022b473c {
  u8 pad_00[0x34];
  int unk_34;
  u8 pad_38[0x1c];
  int unk_54;
  int unk_58;
};

void func_ov004_022b473c(Unk_022b473c* a0) {
  a0->unk_54 = -0x100;
  a0->unk_58 = 128;
  a0->unk_34 = 0;
}
