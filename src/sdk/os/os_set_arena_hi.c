#include "types.h"

typedef struct Unk_0210e270_0 Unk_0210e270_0;

struct Unk_0210e270_0 {
  u8 pad_00[0x27ffdc4];
  int unk_27ffdc4;
};

void OS_SetArenaHi(int a0, int a1) {
  ((Unk_0210e270_0*)(a0 << 2))->unk_27ffdc4 = a1;
}
