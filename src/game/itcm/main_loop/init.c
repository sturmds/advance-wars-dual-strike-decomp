#include "types.h"
#include "game/main_loop.h"

extern MainLoop gMainLoop;

void func_01ff9ba0(void);

void MainLoop_Init(void) {
  gMainLoop.screenCallbacks[0] = NULL;
  gMainLoop.screenCallbacks[1] = NULL;
  gMainLoop.vblankCallback = NULL;
  gMainLoop.unk_24 = 1;
  /* the reset combo (L, R, Start, Select) counts as already held, so holding it at boot does nothing */
  gMainLoop.prevKeys = 0x30c;
  func_01ff9ba0();
}
