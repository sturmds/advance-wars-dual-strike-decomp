#include "types.h"

typedef struct Unk_0207a0d0_0 Unk_0207a0d0_0;

struct Unk_0207a0d0_0 {
  u8 pad_00[0x2c];
  int unk_2c;
};

extern Unk_0207a0d0_0* data_0215fe84;

int func_0207a0d0(void) {
  Unk_0207a0d0_0* t0 = data_0215fe84;
  if (t0->unk_2c == 1) {
    t0->unk_2c = 0;
    return 1;
  }
  return 0;
}
