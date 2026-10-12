#include "types.h"

typedef struct Unk_027cdf20_a1 Unk_027cdf20_a1;

struct Unk_027cdf20_a1 {
  u8 pad_00[0x2];
  u16 unk_2;
};

void WifiRam_StoreFourKeyBlocks(int, void*, int);

void func_arm7_027cdf20(int a0, Unk_027cdf20_a1* a1) {
  a1->unk_2 = 1;
  WifiRam_StoreFourKeyBlocks(a0 + 16, a1, 1);
}
