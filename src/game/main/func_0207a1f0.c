#include "types.h"

typedef struct Unk_0207a1f0_0 Unk_0207a1f0_0;

struct Unk_0207a1f0_0 {
  u8 pad_00[0x2];
  u16 unk_2;
};

extern Unk_0207a1f0_0* data_0215fe84;

int func_0207a1f0(void) { return data_0215fe84->unk_2 & 255; }
