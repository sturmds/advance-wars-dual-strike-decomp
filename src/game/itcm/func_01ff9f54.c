#include "types.h"

typedef struct Unk_gMainLoop Unk_gMainLoop;

struct Unk_gMainLoop {
  u8 pad_00[0x1c];
  int unk_1c;
};

extern Unk_gMainLoop gMainLoop;

void func_01ff9f54(int a0) { gMainLoop.unk_1c = a0; }
