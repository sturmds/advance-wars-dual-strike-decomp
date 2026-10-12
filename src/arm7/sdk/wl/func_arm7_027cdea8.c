#include "types.h"

typedef struct Unk_027cdea8 Unk_027cdea8;
typedef struct Unk_027cdea8_a1 Unk_027cdea8_a1;

struct Unk_027cdea8 {
  u8 pad_00[0x10];
  u16 unk_10;
};

struct Unk_027cdea8_a1 {
  u8 pad_00[0x2];
  u16 unk_2;
};

void WifiParam_StoreBeaconsPerRun(int, void*, int);

void func_arm7_027cdea8(Unk_027cdea8* a0, Unk_027cdea8_a1* a1) {
  a1->unk_2 = 1;
  WifiParam_StoreBeaconsPerRun(a0->unk_10, a1, 1);
}
