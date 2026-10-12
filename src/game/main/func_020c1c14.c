#include "types.h"

typedef struct Unk_020c1c14_0 Unk_020c1c14_0;
typedef struct Unk_020c1c14_1 Unk_020c1c14_1;

struct Unk_020c1c14_0 {
  u8 pad_00[0x48];
  Unk_020c1c14_1* unk_48[1];
};

struct Unk_020c1c14_1 {
  u8 pad_00[0x34];
  u16 unk_34;
};

extern Unk_020c1c14_0* data_027c027c;
extern int data_027c0284;

void func_020c1c14(void) {
  Unk_020c1c14_0* t0 = data_027c027c;
  int t1 = data_027c0284;
  t0->unk_48[t1]->unk_34 = 0;
}
