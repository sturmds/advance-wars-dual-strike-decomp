#include "types.h"

typedef struct Unk_0207bd94_0 Unk_0207bd94_0;

struct Unk_0207bd94_0 {
  u8 pad_00[0x14];
  int unk_14;
};

extern Unk_0207bd94_0* data_0215fe8c;

int func_0207bd94(void) { return data_0215fe8c->unk_14; }
