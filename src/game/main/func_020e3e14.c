#include "types.h"

typedef struct Unk_020e3e14 Unk_020e3e14;

struct Unk_020e3e14 {
  u8 pad_00[0x29];
  u8 unk_29;
  u8 unk_2a;
  u8 unk_2b;
};

void func_020e3e14(Unk_020e3e14* a0) {
  a0->unk_29 = 1;
  a0->unk_2a = 1;
  a0->unk_2b = 0;
}
