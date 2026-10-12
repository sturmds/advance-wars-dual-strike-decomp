#include "sdk/rtc.h"

/* Reads the time (hour, minute, second; BCD) into `data`. */
void func_arm7_027d6128(u8* data) {
  func_arm7_037fe3e0(0x8000);
  func_arm7_027d5b18(CLOCK_XFER_FROM_CHIP, CLOCK_SEL_TIME_ONLY, data, 3);
}
