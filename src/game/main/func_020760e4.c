#include "types.h"

typedef struct Unk_020760e4 Unk_020760e4;

struct Unk_020760e4 {
  u8 pad_00[0x52];
  u16 unk_52;
};

void func_02075e08(int);

void func_020760e4(Unk_020760e4* a0) { func_02075e08(a0->unk_52); }
