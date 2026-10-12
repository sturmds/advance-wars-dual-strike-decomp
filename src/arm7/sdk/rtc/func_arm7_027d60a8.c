#include "sdk/rtc.h"

int func_arm7_027d60a8(u8* data) {
  RtcIntRegister reg;

  func_arm7_027d5e98(&reg);
  if ((reg.mode & 0xb) != 1) {
    return 0;
  }
  func_arm7_027d5b18(CLOCK_XFER_FROM_CHIP, CLOCK_SEL_INT1_SETTING, data, 1);
  return 1;
}
