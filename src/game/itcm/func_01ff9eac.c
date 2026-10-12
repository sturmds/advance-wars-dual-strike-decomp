#include "types.h"

typedef struct Unk_gMainLoop Unk_gMainLoop;

struct Unk_gMainLoop {
  u8 pad_00[0x10];
  int unk_10;
};

extern Unk_gMainLoop gMainLoop;

int func_01ff9eac(void) { return gMainLoop.unk_10; }
