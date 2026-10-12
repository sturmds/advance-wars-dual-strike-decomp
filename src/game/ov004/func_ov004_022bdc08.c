#include "types.h"

typedef struct Unk_022bdc08 Unk_022bdc08;

struct Unk_022bdc08 {
  u8 pad_00[0x1bf4];
  int unk_1bf4;
  int unk_1bf8;
  int unk_1bfc;
  int unk_1c00;
  int unk_1c04;
  int unk_1c08;
};

int func_ov004_022bdc08(Unk_022bdc08* a0) {
  a0->unk_1bf8 = 0;
  a0->unk_1bfc = 0;
  a0->unk_1c00 = 0;
  a0->unk_1c04 = 0;
  a0->unk_1bf4 = 0;
  a0->unk_1c08 = 0;
  return (int)a0 + 0x1000;
}
