#include "types.h"

typedef struct Unk_02364300_0 Unk_02364300_0;
typedef struct Unk_02364300 Unk_02364300;

struct Unk_02364300_0 {
  u8 pad_00[0x17];
  u8 unk_17;
};

struct Unk_02364300 {
  u8 pad_00[0x60];
  Unk_02364300_0* unk_60;
};

int func_ov003_02364300(Unk_02364300* a0) {
  Unk_02364300_0* t0 = a0->unk_60;
  u8 t1 = t0->unk_17;
  int t2 = t1 & -65;
  t0->unk_17 = t2;
  return t2;
}
