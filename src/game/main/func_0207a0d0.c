#include "types.h"
#include "game/Unk_0215fe84.h"

int func_0207a0d0(void) {
  Unk_0215fe84* t0 = data_0215fe84;
  if (t0->unk_2c == 1) {
    t0->unk_2c = 0;
    return 1;
  }
  return 0;
}
