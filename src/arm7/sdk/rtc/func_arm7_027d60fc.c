#include "sdk/rtc.h"

/* Writes the time (hour, minute, second; BCD) from `data`. */
void func_arm7_027d60fc(u8 *data)
{
    func_arm7_037fe3e0(0x8000);
    func_arm7_027d5b18(CLOCK_XFER_TO_CHIP, CLOCK_SEL_TIME_ONLY, data, 3);
}
