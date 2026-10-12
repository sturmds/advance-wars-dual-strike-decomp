#include "types.h"

typedef struct Unk_02105814_0 Unk_02105814_0;

struct Unk_02105814_0 {
  u8 pad_00[0x40];
  u8 unk_40;
};

void func_02105814(Unk_02105814_0** a0, int a1) {
  Unk_02105814_0* t0 = *a0;
  if ((int)t0 != 0) {
    t0->unk_40 = a1;
  }
}
