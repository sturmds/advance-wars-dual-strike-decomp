#include "types.h"

typedef struct Unk_gMainLoop Unk_gMainLoop;

struct Unk_gMainLoop {
  u8 pad_00[0x14];
  int unk_14;
};

extern Unk_gMainLoop gMainLoop;

void* func_01ff9ba0(void) {
  gMainLoop.unk_14 = 0;
  return &gMainLoop;
}
