#include "types.h"

typedef struct Unk_027c83c8_0 Unk_027c83c8_0;

struct Unk_027c83c8_0 {
  u8 pad_00[0x5c8];
  u16 unk_5c8;
};

void func_arm7_027c83c8(int a0) {
  Unk_027c83c8_0* t0 = *((Unk_027c83c8_0**)0x380fff4);
  t0->unk_5c8 = a0;
}
