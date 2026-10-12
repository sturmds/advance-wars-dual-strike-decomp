#include "types.h"

typedef struct Unk_02077f34_0 Unk_02077f34_0;

struct Unk_02077f34_0 {
  u8 pad_00[0x18];
  int unk_18;
};

extern Unk_02077f34_0* data_0215fe84;

void func_02077f34(int a0) { data_0215fe84->unk_18 = a0; }
