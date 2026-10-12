#include "sdk/rtc.h"

/* Resets the clock: writes status 1 with the reset bit set. The other
 * bits of the written byte are whatever the stack held. */
void func_arm7_027d6304(void) {
  RtcStatus1 status;

  func_arm7_037fe3e0(0x8000);
  status.reset = 1;
  func_arm7_027d5b18(CLOCK_XFER_TO_CHIP, CLOCK_SEL_STATUS_A, &status, 1);
}
