#include "types.h"

typedef struct Unk_02059280 Unk_02059280;

struct Unk_02059280 {
  u8 pad_00[0x4c];
  u16 unk_4c;
  u8 pad_4e[0xa];
  int unk_58;
  int unk_5c;
};

void func_02059280(Unk_02059280* a0) {
  a0->unk_4c = 0;
  a0->unk_58 = 0;
  a0->unk_5c = 0;
}
