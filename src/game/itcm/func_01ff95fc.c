#include "types.h"
#include "game/main_loop.h"

extern MainLoop gMainLoop;

void* func_01ff95fc(void) {
  gMainLoop.unk_24 = 1;
  return &gMainLoop;
}
