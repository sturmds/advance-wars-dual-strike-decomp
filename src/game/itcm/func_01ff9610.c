#include "types.h"
#include "game/main_loop.h"

extern MainLoop gMainLoop;

void* func_01ff9610(void) {
  gMainLoop.unk_24 = 0;
  return &gMainLoop;
}
