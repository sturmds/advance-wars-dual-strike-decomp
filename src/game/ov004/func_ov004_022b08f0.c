#include "types.h"

typedef struct Unk_022b08f0 Unk_022b08f0;

struct Unk_022b08f0 {
  u8 pad_00[0x34];
  int unk_34;
  u8 pad_38[0xc];
  int unk_44;
};

void func_ov004_022b08f0(Unk_022b08f0* a0) {
  a0->unk_44 = 5;
  a0->unk_34 = 0;
}
