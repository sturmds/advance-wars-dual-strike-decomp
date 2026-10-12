#include "types.h"

typedef struct Unk_0216e2b0 Unk_0216e2b0;

struct Unk_0216e2b0 {
  u8 pad_00[0x1];
  u8 unk_1;
};

extern Unk_0216e2b0 data_0216e2b0;

int func_02007710(int a0) {
  u8 t0 = data_0216e2b0.unk_1;
  int t1 = t0 & -16;
  int t2 = a0 & 15;
  int t3 = t1 | t2;
  data_0216e2b0.unk_1 = t3;
  return t3;
}
