#include "types.h"

typedef struct Unk_02027aa8 Unk_02027aa8;

struct Unk_02027aa8 {
  u8 pad_00[0x40];
  int unk_40;
  int unk_44;
};

void func_02027aa8(Unk_02027aa8* a0, int a1, int a2) {
  a0->unk_40 = a1;
  a0->unk_44 = a2;
}
