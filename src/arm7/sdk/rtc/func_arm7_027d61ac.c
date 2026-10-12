#include "sdk/rtc.h"

/* Writes the date and the time. */
void func_arm7_027d61ac(RtcDateTime* value) {
  func_arm7_037fe3e0(0x8000);
  func_arm7_027d5b18(CLOCK_XFER_TO_CHIP, CLOCK_SEL_DATE_AND_TIME, value, 7);
}
