#include "types.h"

typedef struct Unk_0210e4a8_0 Unk_0210e4a8_0;

struct Unk_0210e4a8_0 {
  u8 pad_00[0x27ffdc4];
  int unk_27ffdc4;
};

int OS_GetArenaHi(int a0) { return ((Unk_0210e4a8_0*)(a0 << 2))->unk_27ffdc4; }
