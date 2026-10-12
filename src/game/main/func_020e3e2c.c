#include "types.h"

typedef struct Unk_020e3e2c Unk_020e3e2c;

struct Unk_020e3e2c {
  u8 pad_00[0x2c];
  u8 unk_2c;
};

void PowerHighlight_LoadEffect(int);

void func_020e3e2c(Unk_020e3e2c* a0) { PowerHighlight_LoadEffect(a0->unk_2c); }
