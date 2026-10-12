#include "types.h"

typedef struct Unk_0211be3c_0 Unk_0211be3c_0;

struct Unk_0211be3c_0 {
  u8 pad_00[0xc];
  u16 unk_c;
};

extern Unk_0211be3c_0* data_022aced8;

u16 func_0211be3c(void) { return data_022aced8->unk_c; }
