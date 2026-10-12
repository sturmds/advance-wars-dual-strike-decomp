#include "types.h"

typedef struct Unk_gMainLoop Unk_gMainLoop;

struct Unk_gMainLoop {
  u8 pad_00[0x18];
  int unk_18;
};

extern Unk_gMainLoop gMainLoop;

void func_01ff9f8c(int a0) { gMainLoop.unk_18 = a0; }
