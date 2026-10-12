#include "types.h"

typedef struct Unk_0207b4c8_0 Unk_0207b4c8_0;

struct Unk_0207b4c8_0 {
  u8 pad_00[0xb];
  u8 unk_b;
};

extern Unk_0207b4c8_0* data_0215fe8c;

int func_0207b4c8(void) { return (data_0215fe8c->unk_b != 0) ? 1 : 0; }
