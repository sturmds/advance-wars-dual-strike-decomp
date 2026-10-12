#include "types.h"

typedef struct Unk_022a7fac Unk_022a7fac;

struct Unk_022a7fac {
  u8 pad_00[0x2];
  s8 unk_2;
};

extern Unk_022a7fac data_022a7fac;

s8 func_021023e4(void) { return data_022a7fac.unk_2; }
