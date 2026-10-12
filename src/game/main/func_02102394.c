#include "types.h"

typedef struct Unk_022a7fac Unk_022a7fac;

struct Unk_022a7fac {
  u8 pad_00[0x1];
  s8 unk_1;
};

extern Unk_022a7fac data_022a7fac;

s8 func_02102394(void) { return data_022a7fac.unk_1; }
