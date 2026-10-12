#include "sdk/rtc.h"

int func_arm7_027d5fb4(u8* data) {
  RtcIntRegister reg;

  func_arm7_027d5e98(&reg);
  if (reg.mode != 4) {
    return 0;
  }
  func_arm7_027d5b18(CLOCK_XFER_TO_CHIP, CLOCK_SEL_INT1_SETTING, data, 3);
  return 1;
}
