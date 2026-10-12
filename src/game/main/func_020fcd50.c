#include "types.h"
#include "game/brightness_fade.h"

Task* func_020fcd50(void* parentOrList, int frames) {
  return BrightnessFade_Start(parentOrList, 1, 1, 0, frames);
}
