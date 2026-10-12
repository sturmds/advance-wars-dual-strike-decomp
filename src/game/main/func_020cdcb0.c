#include "types.h"

typedef struct Unk_020cdcb0 Unk_020cdcb0;

struct Unk_020cdcb0 {
  u8 pad_00[0x8];
  int unk_8;
  u8 pad_0c[0x29];
  s8 unk_35;
};

void func_020cdcb0(Unk_020cdcb0* a0) {
  s8 t0 = a0->unk_35;
  if (t0 != 0) {
    int t1 = t0 - 1;
    a0->unk_35 = t1;
  } else {
    a0->unk_8 = 0;
  }
}
