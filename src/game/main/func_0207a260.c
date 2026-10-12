#include "types.h"

typedef struct Unk_0207a260_0 Unk_0207a260_0;

struct Unk_0207a260_0 {
  u8 pad_00[0x6];
  s8 unk_6;
};

extern Unk_0207a260_0* data_0215fe84;

int func_0207a260(void) { return (u16)data_0215fe84->unk_6; }
