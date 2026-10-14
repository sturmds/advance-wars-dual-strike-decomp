#include "types.h"
#include "game/main_loop.h"

extern MainLoop gMainLoop;

void* func_01ff9ba0(void) {
  gMainLoop.unk_14 = 0;
  return &gMainLoop;
}
