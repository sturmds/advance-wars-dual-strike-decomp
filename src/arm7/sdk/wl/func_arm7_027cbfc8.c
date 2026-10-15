#include "types.h"
#include "sdk/wl.h"

typedef struct Unk_027cbfc8_0 Unk_027cbfc8_0;

struct Unk_027cbfc8_0 {
  u8 pad_00[0x3d8];
  u16 unk_3d8;
};

void func_arm7_027cbfc8(void) {
  Unk_027cbfc8_0* t0 = *((Unk_027cbfc8_0**)0x380fff4);
  t0->unk_3d8 = 131;
  WifiTask_Schedule(2, 5);
}
