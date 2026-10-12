#include "sdk/rtc.h"

void func_arm7_027d5e14(u8 *value)
{
    func_arm7_037fe3e0(0x8000);
    func_arm7_027d5b18(CLOCK_XFER_TO_CHIP, CLOCK_SEL_ADDR3, value, 1);
}
