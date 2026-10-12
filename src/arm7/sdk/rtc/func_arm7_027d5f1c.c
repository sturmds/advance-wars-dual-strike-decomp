#include "sdk/rtc.h"

int func_arm7_027d5f1c(u8* data) {
  RtcIntRegister reg;

  func_arm7_027d5e98(&reg);
  if (!reg.int2Enabled) {
    return 0;
  }
  func_arm7_027d5b18(CLOCK_XFER_TO_CHIP, CLOCK_SEL_INT2_ALARM, data, 3);
  return 1;
}
