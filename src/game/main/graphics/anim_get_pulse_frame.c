#include "types.h"

int MainLoop_GetFrameCount(void);

/* Frame 0 to 2 of a 32-frame loop driven by the main loop frame count:
 * 0 for 12 frames, 1 for 4, 2 for 12, 1 for 4. */
int Anim_GetPulseFrame(void) {
  u16 phase = MainLoop_GetFrameCount() % 32;

  if (phase < 12) {
    return 0;
  }
  if (phase < 16) {
    return 1;
  }
  if (phase < 28) {
    return 2;
  }
  return 1;
}
