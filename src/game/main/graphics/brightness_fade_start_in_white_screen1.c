#include "types.h"
#include "game/brightness_fade.h"

Task* BrightnessFade_StartInWhiteScreen1(void* parentOrList, int frames) {
  return BrightnessFade_Start(parentOrList, 0, 1, 1, frames);
}
