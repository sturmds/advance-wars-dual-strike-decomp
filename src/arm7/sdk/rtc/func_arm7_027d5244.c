#include "sdk/rtc.h"

/* Their definitions take u8 *; these are the registers they move. */
void func_arm7_027d5ef0(RtcStatus1 *out);
void func_arm7_027d5ec4(RtcStatus1 *value);
void func_arm7_027d5e6c(RtcIntRegister *value);

void func_arm7_027d61ac(RtcDateTime *value);
void func_arm7_027d61d8(RtcDateTime *out);
void func_arm7_027d6204(int hour24);
u32 func_arm7_027d5150(u32 bcd);
u32 func_arm7_027d51c8(u32 year, u32 month, u32 day);

/*
 * Boot check: resets the clock if it lost power or is in test mode, turns
 * off interrupts that fired while the console was off, and rewrites the
 * weekday when it does not fit the date.
 */
void func_arm7_027d5244(void)
{
    RtcStatus1 status;
    RtcIntRegister intReg;
    RtcDateTime now;
    u32 year;
    u32 month;
    u32 day;
    u32 weekday;

    func_arm7_027d5ef0(&status);
    func_arm7_027d5e98(&intReg);
    if (status.powerOn || status.lowVoltage || intReg.test) {
        status.reset = 1;
        func_arm7_027d5ec4(&status);
    }
    if (status.int1 || status.int2) {
        intReg.mode = 0;
        intReg.alarm1Enabled = 0;
        func_arm7_027d5e6c(&intReg);
    }
    func_arm7_027d61d8(&now);
    year = func_arm7_027d5150(now.year);
    month = func_arm7_027d5150(now.month);
    day = func_arm7_027d5150(now.day);
    weekday = func_arm7_027d51c8(year + 2000, month, day);
    if (now.weekday != weekday) {
        now.weekday = weekday;
        func_arm7_027d61ac(&now);
    }
    func_arm7_027d6204(1);
}
