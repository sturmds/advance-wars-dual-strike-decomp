#include "types.h"
#include "game/brightness_fade.h"

Task* func_020fcda0(void* parentOrList, int frames) {
  return BrightnessFade_Start(parentOrList, 0, 1, 0, frames);
}
