#include "types.h"
#include "game/main_loop.h"

extern MainLoop gMainLoop;

void* func_01ff9ebc(void) {
  gMainLoop.unk_10 = 0;
  return &gMainLoop;
}
