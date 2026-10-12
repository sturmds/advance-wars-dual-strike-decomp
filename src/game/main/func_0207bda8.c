#include "types.h"

typedef struct Unk_0207bda8_0 Unk_0207bda8_0;

struct Unk_0207bda8_0 {
  u8 pad_00[0xf];
  u8 unk_f;
};

extern Unk_0207bda8_0* data_0215fe8c;

void func_0207bda8(int a0) { data_0215fe8c->unk_f = a0; }
