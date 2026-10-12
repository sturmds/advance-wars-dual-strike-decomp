#include "types.h"
#include "game/ov003_unit.h"

int func_ov003_02363e40(void) {
  Ov003State* state = data_ov003_02364928;
  u8 value = state->phases;
  int t2 = value & -241;
  int t3 = t2 | 16;
  state->phases = t3;
  return t3;
}
