#include "types.h"

typedef struct Unk_020ebb98 Unk_020ebb98;

struct Unk_020ebb98 {
  u8 pad_00[0x2c];
  int unk_2c;
  int unk_30;
};

void func_020f5888(int a0, int a1);

void func_020ebb98(Unk_020ebb98* a0) {
  int t0 = a0->unk_2c;
  func_020f5888((s16)t0, (s16)a0->unk_30);
}
