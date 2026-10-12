#include "types.h"

/* Step counter of each side's hit shake, read by Battle_StepSideShake. */
extern u8 data_ov002_0236bbdc[2];

/* Starts the shake of a side (0 or 1) at its first step. */
void Battle_StartSideShake(u32 side) {
  if (side < 2) {
    data_ov002_0236bbdc[side] = 1;
  }
}
