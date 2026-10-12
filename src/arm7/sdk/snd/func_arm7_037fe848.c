#include "types.h"

typedef struct Unk_037fe848_0 Unk_037fe848_0;

struct Unk_037fe848_0 {
  u8 pad_00[0x4000408];
  u16 unk_4000408;
};

void func_arm7_037fe848(int a0, int a1) {
  ((Unk_037fe848_0*)(a0 << 4))->unk_4000408 = 0x10000 - a1;
}
