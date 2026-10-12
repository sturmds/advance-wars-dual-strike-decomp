#include "types.h"

typedef struct Unk_0201c87c Unk_0201c87c;

struct Unk_0201c87c {
  u8 pad_00[0x8];
  void* unk_8;
};

void func_0201c88c(void);

void func_0201c87c(Unk_0201c87c* a0) { a0->unk_8 = (void*)func_0201c88c; }
