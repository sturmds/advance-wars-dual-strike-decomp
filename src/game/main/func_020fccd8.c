#include "types.h"
#include "game/brightness_fade.h"

Task* func_020fccd8(void* parentOrList, int frames) {
  return BrightnessFade_Start(parentOrList, 1, 0, 1, frames);
}
