#include "types.h"

typedef struct Unk_027c90ec Unk_027c90ec;
typedef struct Unk_027c90ec_a1 Unk_027c90ec_a1;

struct Unk_027c90ec {
  u16 unk_0;
  u16 unk_2;
  u16 unk_4;
};

struct Unk_027c90ec_a1 {
  u16 unk_0;
  u16 unk_2;
  u16 unk_4;
};

void func_arm7_027c90ec(Unk_027c90ec* a0, Unk_027c90ec_a1* a1) {
  a0->unk_0 = a1->unk_0;
  a0->unk_2 = a1->unk_2;
  a0->unk_4 = a1->unk_4;
}
