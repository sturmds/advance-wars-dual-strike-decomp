#include "types.h"

typedef struct Unk_02116f74_0 Unk_02116f74_0;

struct Unk_02116f74_0 {
  u8 pad_00[0x18];
  int unk_18[1];
};

extern Unk_02116f74_0* data_022ac32c;

int func_02116f74(int a0, int a1) {
  Unk_02116f74_0* t0 = data_022ac32c;
  t0->unk_18[a0] = a1;
  return (int)t0 + (a0 << 2);
}
