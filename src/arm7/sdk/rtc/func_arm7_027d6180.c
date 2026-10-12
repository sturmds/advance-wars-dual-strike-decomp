#include "sdk/rtc.h"

/* Reads the date only: the first 4 bytes of `out`. */
void func_arm7_027d6180(RtcDateTime* out) {
  func_arm7_037fe3e0(0x8000);
  func_arm7_027d5b18(CLOCK_XFER_FROM_CHIP, CLOCK_SEL_DATE_AND_TIME, out, 4);
}
