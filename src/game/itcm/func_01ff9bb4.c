#include "types.h"
#include "game/main_loop.h"

extern MainLoop gMainLoop;

int func_01ff9bb4(void (*callback)(void)) {
  return (gMainLoop.screenCallbacks[0] == callback) ? 1 : 0;
}
