#include "types.h"
#include "game/main_loop.h"

extern MainLoop gMainLoop;

void* MainLoop_CountFrame(void) {
  gMainLoop.frameCount = gMainLoop.frameCount + 1;
  return &gMainLoop;
}
