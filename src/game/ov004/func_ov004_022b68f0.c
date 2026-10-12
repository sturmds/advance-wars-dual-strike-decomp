#include "types.h"

typedef struct Unk_022b68f0 Unk_022b68f0;

struct Unk_022b68f0 {
  u8 pad_00[0x34];
  int unk_34;
  u8 pad_38[0x1c];
  int unk_54;
};

void func_ov004_022b68f0(Unk_022b68f0* a0) {
  a0->unk_54 = 1;
  a0->unk_34 = 0;
}
