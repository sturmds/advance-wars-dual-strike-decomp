#include "types.h"

typedef struct Unk_gMainLoop Unk_gMainLoop;

struct Unk_gMainLoop {
  u8 pad_00[0x1c];
  int unk_1c;
};

extern Unk_gMainLoop gMainLoop;

int func_01ff9f44(void) { return gMainLoop.unk_1c; }
