#include "types.h"
#include "sdk/pxi.h"
#include "sdk/snd.h"

void func_arm7_0380239c(void) {
  Fifo_RegisterTagHandler_Arm7(SOUND_FIFO_TAG, SndArm7_OnPxiReceive);
}
