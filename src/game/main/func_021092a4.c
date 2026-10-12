#include "types.h"

typedef struct Unk_021092a4 Unk_021092a4;

struct Unk_021092a4 {
  u8 pad_00[0x8];
  int unk_8;
  int unk_c;
};

void func_021092a4(Unk_021092a4* a0) {
  int t0 = a0->unk_8;
  int t1 = a0->unk_c;
  if (t0 < t1) {
    int t2 = t0 + 1;
    a0->unk_8 = t2;
  }
}
