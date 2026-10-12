#include "types.h"

typedef struct Unk_0207a120_0 Unk_0207a120_0;

struct Unk_0207a120_0 {
  u8 pad_00[0x24];
  int unk_24;
};

extern Unk_0207a120_0* data_0215fe84;

int func_0207a120(void) {
  Unk_0207a120_0* t0 = data_0215fe84;
  if (t0->unk_24 == 1) {
    t0->unk_24 = 0;
    return 1;
  }
  return 0;
}
