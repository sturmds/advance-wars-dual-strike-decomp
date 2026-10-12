#include "types.h"

typedef struct Unk_02290118 Unk_02290118;

struct Unk_02290118 {
  u8 pad_00[0x8];
  u16 unk_8;
};

extern Unk_02290118 data_02290118;

int func_0210391c(void) { return (data_02290118.unk_8 != 0) ? 1 : 0; }
