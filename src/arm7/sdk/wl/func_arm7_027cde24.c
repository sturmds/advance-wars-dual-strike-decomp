#include "types.h"

typedef struct Unk_027cde24 Unk_027cde24;
typedef struct Unk_027cde24_a1 Unk_027cde24_a1;

struct Unk_027cde24 {
  u8 pad_00[0x10];
  u16 unk_10;
  u16 unk_12;
};

struct Unk_027cde24_a1 {
  u8 pad_00[0x2];
  u16 unk_2;
};

void WifiBaseband_SetCcaParams(int, int, void*);

void func_arm7_027cde24(Unk_027cde24* a0, Unk_027cde24_a1* a1) {
  a1->unk_2 = 1;
  WifiBaseband_SetCcaParams(a0->unk_10, a0->unk_12, a0);
}
