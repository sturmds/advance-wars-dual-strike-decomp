#include "types.h"

typedef struct Unk_02110a90 Unk_02110a90;

struct Unk_02110a90 {
  u8 pad_00[0x38];
  int unk_38;
};

int func_02110a90(Unk_02110a90* a0) { return a0->unk_38; }
