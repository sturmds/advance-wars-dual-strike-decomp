#include "types.h"
#include "game/main_loop.h"

extern MainLoop gMainLoop;

void* func_01ff9ef8(void) {
  gMainLoop.unk_0c = 1;
  return &gMainLoop;
}
