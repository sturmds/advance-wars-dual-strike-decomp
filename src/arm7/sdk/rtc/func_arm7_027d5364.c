#include "sdk/rtc.h"

/* Their definitions take u8 *; these are the registers they move. */
void func_arm7_027d5ef0(RtcStatus1 *out);
void func_arm7_027d5e6c(RtcIntRegister *value);

/*
 * Alarm interrupt: turns off each interrupt that fired and tells the ARM9
 * which ones did (bit 0 for INT1, bit 1 for INT2).
 */
void func_arm7_027d5364(void)
{
    RtcStatus1 status;
    RtcIntRegister intReg;
    u32 fired;

    func_arm7_027d5ef0(&status);
    if (status.int1 || status.int2) {
        func_arm7_027d5e98(&intReg);
        fired = 0;
        if (status.int1) {
            fired |= 1;
            intReg.mode = 0;
        }
        if (status.int2) {
            fired |= 2;
            intReg.int2Enabled = 0;
        }
        func_arm7_027d5e6c(&intReg);
        func_arm7_027d58a8(RTC_PXI_COMMAND_INTERRUPT, fired);
    }
}
