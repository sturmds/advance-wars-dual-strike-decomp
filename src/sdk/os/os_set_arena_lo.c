#include "types.h"

typedef struct Unk_0210e25c_0 Unk_0210e25c_0;

struct Unk_0210e25c_0 {
  u8 pad_00[0x27ffda0];
  int unk_27ffda0;
};

void OS_SetArenaLo(int a0, int a1) {
  ((Unk_0210e25c_0*)(a0 << 2))->unk_27ffda0 = a1;
}
