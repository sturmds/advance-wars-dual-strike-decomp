#include "types.h"
#include "game/main_loop.h"

extern MainLoop gMainLoop;

void func_01ff9c28(void (*callback)(void)) {
  gMainLoop.vblankCallback = callback;
}
