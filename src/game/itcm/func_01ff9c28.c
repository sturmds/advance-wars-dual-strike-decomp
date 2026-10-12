#include "types.h"

typedef struct Unk_gMainLoop Unk_gMainLoop;

struct Unk_gMainLoop {
  u8 pad_00[0x8];
  int unk_8;
};

extern Unk_gMainLoop gMainLoop;

void func_01ff9c28(int a0) { gMainLoop.unk_8 = a0; }
