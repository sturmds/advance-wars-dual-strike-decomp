#include "types.h"

typedef struct Unk_gMainLoop Unk_gMainLoop;

struct Unk_gMainLoop {
  u8 pad_00[0x24];
  int unk_24;
};

extern Unk_gMainLoop gMainLoop;

void* func_01ff95fc(void) {
  gMainLoop.unk_24 = 1;
  return &gMainLoop;
}
