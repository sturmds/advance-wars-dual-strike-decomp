#include "types.h"
#include "game/Ov003Fx.h"

s32 HwMod32(s32 numerator, s32 denominator);
s32 MainLoop_GetFrameCount(void);

#define ANIM data_ov003_02364a84

/* Sets the two counters in Ov003Fx.unk_0c from the global frame count: the low
 * nibble goes 0, 1, 2, 1 at frames 0, 17, 25 and 42 of a 50-frame loop; the high
 * nibble steps 0 to 2 every 8 frames. What they animate is not known. The u32
 * cast on the switch is needed to match. */
void Ov003_StepFrameCycles(void) {
  switch ((u32)HwMod32(MainLoop_GetFrameCount(), 50)) {
    case 0:
      ANIM->unk_0c_0 = 0;
      break;
    case 17:
      ANIM->unk_0c_0 = 1;
      break;
    case 25:
      ANIM->unk_0c_0 = 2;
      break;
    case 42:
      ANIM->unk_0c_0 = 1;
      break;
  }
  ANIM->unk_0c_4 = HwMod32(MainLoop_GetFrameCount() >> 3, 3);
}
