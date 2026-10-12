#include "sdk/rtc.h"

/* Rewrites an alarm hour from 12-hour to 24-hour form (BCD). An hour that
 * is valid in neither form is cleared. */
void func_arm7_027d5b7c(RtcAlarm *alarm)
{
    switch (alarm->hour) {
    case 0x00:
    case 0x01:
    case 0x02:
    case 0x03:
    case 0x04:
    case 0x05:
    case 0x06:
    case 0x07:
    case 0x10:
    case 0x11:
        if (alarm->pm) {
            alarm->hour = alarm->hour + 0x12;
        }
        break;
    case 0x08:
    case 0x09:
        /* BCD: 08 and 09 carry into the tens digit */
        if (alarm->pm) {
            alarm->hour = alarm->hour + 0x18;
        }
        break;
    case 0x12:
    case 0x13:
    case 0x14:
    case 0x15:
    case 0x16:
    case 0x17:
    case 0x18:
    case 0x19:
    case 0x20:
    case 0x21:
    case 0x22:
    case 0x23:
        alarm->pm = 1;
        break;
    default:
        alarm->pm = 0;
        alarm->hour = 0;
        break;
    }
}
