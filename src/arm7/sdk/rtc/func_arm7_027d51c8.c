#include "sdk/rtc.h"

/*
 * Day of the week (0 = Sunday) for a Gregorian date, by Zeller's rule:
 * January and February count as months 13 and 14 of the year before.
 */
u32 func_arm7_027d51c8(u32 year, u32 month, u32 day)
{
    if (month - 1 <= 1) {
        year--;
        month += 12;
    }
    return (year + year / 4 - year / 100 + year / 400 + (13 * month + 8) / 5 + day) % 7;
}
