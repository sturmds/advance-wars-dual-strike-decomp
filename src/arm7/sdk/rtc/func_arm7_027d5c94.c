#include "sdk/rtc.h"

/* Rewrites an alarm hour from 24-hour to 12-hour form (BCD), with pm set
 * for 12 to 23. An hour that is not valid is cleared. */
void func_arm7_027d5c94(RtcAlarm* alarm) {
  switch (alarm->hour) {
    case 0x00:
    case 0x01:
    case 0x02:
    case 0x03:
    case 0x04:
    case 0x05:
    case 0x06:
    case 0x07:
    case 0x08:
    case 0x09:
    case 0x10:
    case 0x11:
      alarm->pm = 0;
      break;
    case 0x12:
    case 0x13:
    case 0x14:
    case 0x15:
    case 0x16:
    case 0x17:
    case 0x18:
    case 0x19:
    case 0x22:
    case 0x23:
      alarm->pm = 1;
      alarm->hour = alarm->hour - 0x12;
      break;
    case 0x20:
    case 0x21:
      /* BCD: 20 and 21 borrow from the tens digit */
      alarm->pm = 1;
      alarm->hour = alarm->hour - 0x18;
      break;
    default:
      alarm->pm = 0;
      alarm->hour = 0;
      break;
  }
}
