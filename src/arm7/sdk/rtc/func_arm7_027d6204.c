#include "sdk/rtc.h"

/* Their definitions take u8 *; these are the registers they move. */
void func_arm7_027d5ef0(RtcStatus1 *out);
void func_arm7_027d5ec4(RtcStatus1 *value);
void func_arm7_027d5b7c(RtcAlarm *alarm);
void func_arm7_027d5c94(RtcAlarm *alarm);

/*
 * Switches the clock to 24-hour mode when bit 0 of `hour24` is set and the
 * clock is not in it yet, and rewrites the hour of both alarms to match.
 * A clear bit does nothing, so the 12-hour branches never run.
 */
void func_arm7_027d6204(u32 hour24)
{
    RtcStatus1 status;
    RtcAlarm alarm;
    /* needed to match: the bit is masked once here and again below */
    u32 wanted = hour24 & 1;

    if ((u16)wanted == 1) {
        func_arm7_027d5ef0(&status);
        if (status.hour24 != (u16)wanted) {
            hour24 &= 1;
            status.hour24 = (u16)hour24;
            func_arm7_027d5ec4(&status);
            func_arm7_027d5b18(CLOCK_XFER_FROM_CHIP, CLOCK_SEL_INT1_SETTING, &alarm, 3);
            if ((u16)hour24 == 0) {
                func_arm7_027d5c94(&alarm);
            } else {
                func_arm7_027d5b7c(&alarm);
            }
            func_arm7_027d5b18(CLOCK_XFER_TO_CHIP, CLOCK_SEL_INT1_SETTING, &alarm, 3);
            func_arm7_027d5b18(CLOCK_XFER_FROM_CHIP, CLOCK_SEL_INT2_ALARM, &alarm, 3);
            if ((u16)wanted == 0) {
                func_arm7_027d5c94(&alarm);
            } else {
                func_arm7_027d5b7c(&alarm);
            }
            func_arm7_027d5b18(CLOCK_XFER_TO_CHIP, CLOCK_SEL_INT2_ALARM, &alarm, 3);
        }
    }
}
