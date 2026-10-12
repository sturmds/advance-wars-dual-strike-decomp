#include "types.h"

typedef struct Unk_020c436c Unk_020c436c;
typedef struct Unk_020c436c_a1 Unk_020c436c_a1;

struct Unk_020c436c {
  s16 unk_0;
  s16 unk_2;
};

struct Unk_020c436c_a1 {
  u16 unk_0;
  u16 unk_2;
};

void func_020c436c(Unk_020c436c* a0, Unk_020c436c_a1* a1) {
  a1->unk_0 = a0->unk_0 << 4;
  a1->unk_2 = a0->unk_2 << 4;
}
