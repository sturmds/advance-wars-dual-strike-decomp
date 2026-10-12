#include "sdk/rtc.h"

void func_arm7_027d5e98(RtcIntRegister *value)
{
    func_arm7_037fe3e0(0x8000);
    func_arm7_027d5b18(CLOCK_XFER_FROM_CHIP, CLOCK_SEL_STATUS_B, value, 1);
}
