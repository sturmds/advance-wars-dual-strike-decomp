#include "types.h"

typedef struct Unk_027cb100_0 Unk_027cb100_0;

struct Unk_027cb100_0 {
  u8 pad_00[0x506];
  u16 unk_506;
};

int func_arm7_027cb100(int a0) {
  Unk_027cb100_0* t0 = *((Unk_027cb100_0**)0x380fff4);
  return (t0->unk_506 >> a0) & 1;
}
