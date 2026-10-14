#include "types.h"
#include "game/Ov003Fx.h"
#include "game/ov003_unit.h"

int Ov003_IsPhase0OrAbove6(void);
s32 HwMod32(s32 numerator, s32 denominator);

/* Steps the overlay's animation counters from the frame counter
 * Ov003State.frameCount, on frames where Ov003_IsPhase0OrAbove6 lets it run. */
void Ov003_StepAnims(void) {
  if (Ov003_IsPhase0OrAbove6() == 0) {
    return;
  }

  data_ov003_02364a84->mode = (data_ov003_02364928->frameCount >> 2) & 15;

  if (HwMod32(data_ov003_02364928->frameCount, 20) == 0) {
    data_ov003_02364a84->unk_0b_0 = 1;
    data_ov003_02364a84->unk_0b_2++;
    data_ov003_02364a84->unk_0b_2 %= 4;
  }
  if (HwMod32(data_ov003_02364928->frameCount, 14) == 0) {
    data_ov003_02364a84->unk_0a_0 = 1;
    data_ov003_02364a84->unk_0a_2++;
    data_ov003_02364a84->unk_0a_2 %= 8;
  }

  /* 0, 1, 2, 3, 2, 1 over 100 frames; the cast gives the unsigned
     * compares of the original */
  switch ((u32)HwMod32(data_ov003_02364928->frameCount, 100)) {
    case 0:
      data_ov003_02364a84->unk_09_0 = 1;
      data_ov003_02364a84->unk_09_2 = 0;
      break;
    case 22:
      data_ov003_02364a84->unk_09_0 = 1;
      data_ov003_02364a84->unk_09_2 = 1;
      break;
    case 36:
      data_ov003_02364a84->unk_09_0 = 1;
      data_ov003_02364a84->unk_09_2 = 2;
      break;
    case 50:
      data_ov003_02364a84->unk_09_0 = 1;
      data_ov003_02364a84->unk_09_2 = 3;
      break;
    case 72:
      data_ov003_02364a84->unk_09_0 = 1;
      data_ov003_02364a84->unk_09_2 = 2;
      break;
    case 86:
      data_ov003_02364a84->unk_09_0 = 1;
      data_ov003_02364a84->unk_09_2 = 1;
      break;
  }
}
