#include "types.h"

typedef struct Unk_0207a1ac_0 Unk_0207a1ac_0;

struct Unk_0207a1ac_0 {
  u8 pad_00[0x10];
  int unk_10;
};

extern Unk_0207a1ac_0* data_0215fe84;

int func_0207a1ac(void) { return data_0215fe84->unk_10; }
