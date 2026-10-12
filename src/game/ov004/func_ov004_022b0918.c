#include "types.h"

typedef struct Unk_022b0918 Unk_022b0918;

struct Unk_022b0918 {
  u8 pad_00[0x34];
  int unk_34;
  u8 pad_38[0xc];
  int unk_44;
};

void func_ov004_022b0918(Unk_022b0918* a0) {
  a0->unk_44 = 3;
  a0->unk_34 = 0;
}
