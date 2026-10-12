#include "types.h"

typedef struct Unk_020e3dfc Unk_020e3dfc;

struct Unk_020e3dfc {
  u8 pad_00[0x29];
  u8 unk_29;
  u8 unk_2a;
  u8 unk_2b;
};

void func_020e3dfc(Unk_020e3dfc* a0) {
  a0->unk_29 = 1;
  a0->unk_2a = 1;
  a0->unk_2b = 0;
}
