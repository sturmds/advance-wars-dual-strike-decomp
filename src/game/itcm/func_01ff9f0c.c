#include "types.h"

typedef struct Unk_gMainLoop Unk_gMainLoop;

struct Unk_gMainLoop {
  u8 pad_00[0x20];
  int unk_20;
};

extern Unk_gMainLoop gMainLoop;

int func_01ff9f0c(void) { return gMainLoop.unk_20; }
