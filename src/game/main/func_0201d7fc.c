#include "types.h"

typedef struct Unk_0201d7fc_0 Unk_0201d7fc_0;

struct Unk_0201d7fc_0 {
  u8 pad_00[0x6a];
  u16 unk_6a;
};

extern Unk_0201d7fc_0* data_027c0280;

int func_0201d7fc(void) {
  Unk_0201d7fc_0* t0 = data_027c0280;
  u16 t1 = t0->unk_6a;
  int t2 = t1 & -17;
  t0->unk_6a = t2;
  return t2;
}
