#include "types.h"
#include "game/brightness_fade.h"

/* Marks the fade as running from its first step. */
void func_020fccc4(BrightnessFadeTask* fade) {
  fade->step = 0;
  fade->active = 1;
}
