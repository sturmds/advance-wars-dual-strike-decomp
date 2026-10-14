#include "types.h"
#include "game/main_loop.h"

extern MainLoop gMainLoop;

void* func_01ff9ee4(void) {
  gMainLoop.unk_0c = 0;
  return &gMainLoop;
}
