#include "types.h"

typedef struct Unk_022b092c Unk_022b092c;

struct Unk_022b092c {
  u8 pad_00[0x34];
  int unk_34;
  u8 pad_38[0xc];
  int unk_44;
};

void func_ov004_022b092c(Unk_022b092c* a0) {
  a0->unk_44 = 2;
  a0->unk_34 = 0;
}
