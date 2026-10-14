#include "types.h"
#include "game/Unk_0215fe84.h"

extern Unk_0215fe84* data_0215fe84;

int func_0207a0f8(void) {
  Unk_0215fe84* t0 = data_0215fe84;
  if (t0->unk_28 == 1) {
    t0->unk_28 = 0;
    return 1;
  }
  return 0;
}
