#include "types.h"
#include "game/battle_side_shake.h"

void func_ov002_02353218(void) {
  int i;

  for (i = 0; i < BATTLE_SIDE_COUNT; i++) {
    data_ov002_0236bbdc[i] = 0;
  }
}
