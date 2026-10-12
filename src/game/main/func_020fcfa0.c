#include "types.h"
#include "game/brightness_fade.h"

void BrightnessFade_Update(BrightnessFadeTask* fade);

void func_020fcfa0(BrightnessFadeTask* fade) { BrightnessFade_Update(fade); }
