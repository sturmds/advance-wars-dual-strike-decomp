#include "types.h"
#include "game/screen.h"

void EffectScene_SetMode(int value);

void func_020f216c(void) {
  SceneMode_Request(7);
  EffectScene_SetMode(2);
}
