#include "types.h"
#include "game/main_loop.h"

extern MainLoop gMainLoop;

void* func_01ff9ed0(void) {
  gMainLoop.unk_10 = 1;
  return &gMainLoop;
}
