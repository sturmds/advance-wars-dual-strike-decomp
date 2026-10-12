#include "sdk/rtc.h"

void func_arm7_027d5ef0(u8 *value)
{
    func_arm7_037fe3e0(0x8000);
    func_arm7_027d5b18(CLOCK_XFER_FROM_CHIP, CLOCK_SEL_STATUS_A, value, 1);
}
