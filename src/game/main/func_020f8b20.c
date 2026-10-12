#include "types.h"

typedef struct Unk_020f8b20_0 Unk_020f8b20_0;

struct Unk_020f8b20_0 {
  u8 pad_00[0x6c];
  u16 unk_6c;
};

extern Unk_020f8b20_0* data_027c0280;

int func_020f8b20(void) {
  Unk_020f8b20_0* t0 = data_027c0280;
  u16 t1 = t0->unk_6c;
  int t2 = t1 | 0x200;
  t0->unk_6c = t2;
  return t2;
}
