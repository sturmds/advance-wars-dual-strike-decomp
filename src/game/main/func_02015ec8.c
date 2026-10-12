#include "types.h"

typedef struct Unk_02015ec8 Unk_02015ec8;

struct Unk_02015ec8 {
  u8 pad_00[0x2c];
  int unk_2c;
  u8 pad_30[0x4];
  u8 unk_34;
  u8 unk_35;
};

int func_02015ec8(Unk_02015ec8* a0) {
  u8 t0 = a0->unk_34;
  int t1 = a0->unk_2c;
  u8 t2 = a0->unk_35;
  int t3 = t0 << 1;
  int t4 = t1 + t3;
  int t5 = t2 << 6;
  int t6 = t4 + t5;
  return t6;
}
