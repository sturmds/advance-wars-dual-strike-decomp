#include "types.h"

typedef struct Unk_02365e90 Unk_02365e90;

struct Unk_02365e90 {
  u8 pad_00[0x4];
  int unk_4;
  u8 pad_08[0xa0];
  void* unk_a8;
};

extern u8 data_ov005_0236b10c[];

void func_ov005_02365e90(Unk_02365e90* a0) {
  a0->unk_a8 = data_ov005_0236b10c;
  a0->unk_4 = 8;
}
