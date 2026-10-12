#include "types.h"
#include "game/brightness_fade.h"

Task* func_020fcd28(void* parentOrList, int frames) {
  return BrightnessFade_Start(parentOrList, 0, 0, 1, frames);
}
