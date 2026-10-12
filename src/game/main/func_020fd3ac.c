#include "types.h"
#include "game/palette_fade.h"

void PaletteFade_Update(PaletteFadeTask* fade, int startLevel, int endLevel);

void func_020fd3ac(PaletteFadeTask* fade) {
  PaletteFade_Update(fade, fade->startLevel, fade->endLevel);
}
