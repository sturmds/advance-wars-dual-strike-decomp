#include "types.h"

typedef struct Unk_0207a184_0 Unk_0207a184_0;

struct Unk_0207a184_0 {
  u8 pad_00[0x20];
  int unk_20;
};

extern Unk_0207a184_0* data_0215fe84;

int func_0207a184(void) {
  Unk_0207a184_0* t0 = data_0215fe84;
  if (t0->unk_20 == 1) {
    t0->unk_20 = 0;
    return 1;
  }
  return 0;
}
