#include "types.h"

typedef struct Unk_020b9914_0 Unk_020b9914_0;
typedef struct Unk_020b9914_1 Unk_020b9914_1;

struct Unk_020b9914_0 {
  u8 pad_00[0x48];
  Unk_020b9914_1* unk_48[1];
};

struct Unk_020b9914_1 {
  u8 pad_00[0xa];
  s16 unk_a;
};

extern Unk_020b9914_0* data_027c027c;
extern int data_027c0284;

s16 func_020b9914(void) {
  Unk_020b9914_0* t0 = data_027c027c;
  int t1 = data_027c0284;
  return t0->unk_48[t1]->unk_a;
}
