#include "types.h"
#include "game/Unk_021694c0.h"

extern Unk_021694c0* data_021694c0;

void EffectScene_SetMode(int value) {
  data_021694c0->effectFrames = 0;
  data_021694c0->effectMode = value;
}
