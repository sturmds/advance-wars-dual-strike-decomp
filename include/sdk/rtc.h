#ifndef SDK_RTC_H
#define SDK_RTC_H

#include "types.h"

/* ARM7 real-time clock. A transfer sends a command byte (mode plus
 * register number in the high nibble) and then moves bytes in or out. */
#define CLOCK_XFER_TO_CHIP 0x06
#define CLOCK_XFER_FROM_CHIP 0x86

#define CLOCK_SEL_STATUS_A 0x00
#define CLOCK_SEL_INT1_SETTING 0x10
#define CLOCK_SEL_ADDR3 0x30
#define CLOCK_SEL_STATUS_B 0x40
#define CLOCK_SEL_INT2_ALARM 0x50
#define CLOCK_SEL_ADDR7 0x70

/* Register 4 (alarm and interrupt mode): the low nibble selects the
 * interrupt mode, bit 6 enables alarm 1. */
typedef struct RtcIntRegister {
    u16 mode : 4;
    u16 unk_4 : 2;
    u16 alarm1Enabled : 1;
    u16 unk_7 : 9;
} RtcIntRegister;

/* Runs one serial transfer of `size` bytes at `data`. */
void func_arm7_027d5b18(int mode, int reg, void *data, int size);

/* Reads the one-byte register 4 into `out`. */
void func_arm7_027d5e98(RtcIntRegister *out);

void func_arm7_037fe3e0(int a0);

#endif
