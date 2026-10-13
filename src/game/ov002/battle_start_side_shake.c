#include "types.h"
#include "game/battle_side_shake.h"

/* Starts the shake of a side (0 or 1) at its first step. */
void Battle_StartSideShake(u32 side) {
  if (side < 2) {
    data_ov002_0236bbdc[side] = 1;
  }
}
