#include "sdk/rtc.h"

/* Reads the date and the time. */
void func_arm7_027d61d8(RtcDateTime *out)
{
    func_arm7_037fe3e0(0x8000);
    func_arm7_027d5b18(CLOCK_XFER_FROM_CHIP, CLOCK_SEL_DATE_AND_TIME, out, 7);
}
