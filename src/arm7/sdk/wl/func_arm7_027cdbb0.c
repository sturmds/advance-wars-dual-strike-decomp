#include "types.h"

typedef struct Unk_027cdbb0 Unk_027cdbb0;
typedef struct Unk_027cdbb0_a1 Unk_027cdbb0_a1;

struct Unk_027cdbb0 {
  u8 pad_00[0x10];
  u16 unk_10;
};

struct Unk_027cdbb0_a1 {
  u8 pad_00[0x2];
  u16 unk_2;
};

void WifiParam_StoreSsid(int, int, void*);

void func_arm7_027cdbb0(Unk_027cdbb0* a0, Unk_027cdbb0_a1* a1) {
  a1->unk_2 = 1;
  WifiParam_StoreSsid(a0->unk_10, (int)a0 + 18, a0);
}
