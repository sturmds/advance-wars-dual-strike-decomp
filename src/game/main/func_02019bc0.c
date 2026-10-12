#include "types.h"

typedef struct Unk_02019bc0_0 Unk_02019bc0_0;
typedef struct Unk_02019bc0_1 Unk_02019bc0_1;

struct Unk_02019bc0_0 {
  u8 pad_00[0x48];
  Unk_02019bc0_1* unk_48[1];
};

struct Unk_02019bc0_1 {
  u8 pad_00[0x85];
  u8 unk_85;
};

extern Unk_02019bc0_0* data_027c027c;
extern int data_027c0284;

void func_02019bc0(void) {
  Unk_02019bc0_0* t0 = data_027c027c;
  int t1 = data_027c0284;
  t0->unk_48[t1]->unk_85 = 1;
}
