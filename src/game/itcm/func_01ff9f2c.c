#include "types.h"
#include "game/main_loop.h"

extern MainLoop gMainLoop;

void* func_01ff9f2c(void) {
  gMainLoop.unk_1c = gMainLoop.unk_1c + 1;
  return &gMainLoop;
}
