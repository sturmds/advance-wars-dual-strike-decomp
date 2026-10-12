#include "types.h"

typedef struct Unk_022b08dc Unk_022b08dc;

struct Unk_022b08dc {
  u8 pad_00[0x34];
  int unk_34;
  u8 pad_38[0xc];
  int unk_44;
};

void func_ov004_022b08dc(Unk_022b08dc* a0) {
  a0->unk_44 = 6;
  a0->unk_34 = 0;
}
