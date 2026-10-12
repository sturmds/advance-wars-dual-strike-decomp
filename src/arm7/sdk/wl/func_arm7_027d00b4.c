#include "types.h"

typedef struct Unk_027d00b4_0 Unk_027d00b4_0;

struct Unk_027d00b4_0 {
  u8 pad_00[0x478];
  u16 unk_478;
};

int func_arm7_027d00b4(void) {
  Unk_027d00b4_0* t0 = *((Unk_027d00b4_0**)0x380fff4);
  *((u16*)0x4808080) = 0;
  t0->unk_478 = 0;
  return (int)t0 + 0x400;
}
