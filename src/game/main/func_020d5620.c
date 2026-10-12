#include "types.h"

typedef struct Unk_020d5620 Unk_020d5620;

struct Unk_020d5620 {
  u8 pad_00[0x58];
  int unk_58;
};

void PowerScreen_Start(void*, int);

void func_020d5620(Unk_020d5620* a0) { PowerScreen_Start(a0, a0->unk_58); }
