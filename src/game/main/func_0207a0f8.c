#include "types.h"

typedef struct Unk_0207a0f8_0 Unk_0207a0f8_0;

struct Unk_0207a0f8_0 {
  u8 pad_00[0x28];
  int unk_28;
};

extern Unk_0207a0f8_0* data_0215fe84;

int func_0207a0f8(void) {
  Unk_0207a0f8_0* t0 = data_0215fe84;
  if (t0->unk_28 == 1) {
    t0->unk_28 = 0;
    return 1;
  }
  return 0;
}
