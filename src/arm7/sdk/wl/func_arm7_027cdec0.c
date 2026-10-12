#include "types.h"

typedef struct Unk_027cdec0 Unk_027cdec0;
typedef struct Unk_027cdec0_a1 Unk_027cdec0_a1;

struct Unk_027cdec0 {
  u8 pad_00[0x10];
  u16 unk_10;
};

struct Unk_027cdec0_a1 {
  u8 pad_00[0x2];
  u16 unk_2;
};

void WifiParam_SetProbeNeedsSsid(int, void*, int);

void func_arm7_027cdec0(Unk_027cdec0* a0, Unk_027cdec0_a1* a1) {
  a1->unk_2 = 1;
  WifiParam_SetProbeNeedsSsid(a0->unk_10, a1, 1);
}
