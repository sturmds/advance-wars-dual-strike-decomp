#include "types.h"

typedef struct Unk_gMainLoop Unk_gMainLoop;

struct Unk_gMainLoop {
  u8 pad_00[0xc];
  int unk_c;
};

extern Unk_gMainLoop gMainLoop;

void* func_01ff9ef8(void) {
  gMainLoop.unk_c = 1;
  return &gMainLoop;
}
