#include "types.h"
#include "sdk/gx.h"
#include "game/screen_other_map.h"

/* Step counter of each side, set to 1 by Battle_StartSideShake. */
extern u8 data_ov002_0236bbdc[2];
/* Vertical offsets of the shake, indexed by step; 0 ends it. Volatile only to
 * match: the original reads the entry again for the scroll write. */
extern volatile s32 data_ov002_023604f4[];

/* One frame of a side's hit shake: moves the 3D scene down by the current
 * offset (MTX_TRANS 0, dy, 0), scrolls the side's layer by it and goes to the next step. */
void Battle_StepSideShake(u32 side) {
  s32 dy;
  u8 step;

  if (side >= 2) {
    return;
  }
  step = data_ov002_0236bbdc[side];
  dy = data_ov002_023604f4[step];
  if (dy == 0) {
    return;
  }
  *REG_MTX_TRANS = 0;
  *REG_MTX_TRANS = dy;
  *REG_MTX_TRANS = 0;
  if (side == 0) {
    data_027c0280->bgScrollY[data_027c0280->layer2Bg] =
        data_ov002_023604f4[step];
  } else {
    data_027c0280->bgScrollY[data_027c0280->layer3Bg] =
        data_ov002_023604f4[step];
  }
  data_ov002_0236bbdc[side]++;
}
