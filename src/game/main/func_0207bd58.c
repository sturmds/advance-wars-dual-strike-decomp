#include "types.h"

typedef struct Unk_0207bd58_0 Unk_0207bd58_0;

struct Unk_0207bd58_0 {
  u8 pad_00[0xd];
  u8 unk_d;
};

extern Unk_0207bd58_0* data_0215fe8c;

void func_0207bd58(int a0) { data_0215fe8c->unk_d = a0; }
