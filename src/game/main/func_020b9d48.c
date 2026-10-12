#include "types.h"

typedef struct Unk_020b9d48_0 Unk_020b9d48_0;
typedef struct Unk_020b9d48_1 Unk_020b9d48_1;

struct Unk_020b9d48_0 {
  u8 pad_00[0x48];
  Unk_020b9d48_1* unk_48[1];
};

struct Unk_020b9d48_1 {
  u8 pad_00[0xc];
  u8 unk_c;
};

extern Unk_020b9d48_0* data_027c027c;
extern int data_027c0284;

void func_020b9d48(void) {
  Unk_020b9d48_0* t0 = data_027c027c;
  int t1 = data_027c0284;
  t0->unk_48[t1]->unk_c = 1;
}
