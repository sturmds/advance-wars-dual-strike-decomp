#include "types.h"

typedef struct Unk_020044a8_0 Unk_020044a8_0;

struct Unk_020044a8_0 {
  u8 pad_00[0x6a];
  u16 unk_6a;
};

extern Unk_020044a8_0* data_027c0280;

int func_020044a8(void) {
  Unk_020044a8_0* t0 = data_027c0280;
  u16 t1 = t0->unk_6a;
  int t2 = t1 | 8;
  t0->unk_6a = t2;
  return t2;
}
