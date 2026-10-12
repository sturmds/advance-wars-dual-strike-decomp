#include "types.h"

typedef struct Unk_0229e148 Unk_0229e148;

struct Unk_0229e148 {
  u8 pad_00[0x16];
  u8 unk_16;
};

extern Unk_0229e148 data_0229e148;

int func_02102378(void) { return (data_0229e148.unk_16 != 0) ? 1 : 0; }
