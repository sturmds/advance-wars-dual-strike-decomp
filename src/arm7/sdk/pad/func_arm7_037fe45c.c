#include "types.h"
#include "sdk/pad.h"

/* The ARM7 OS alarm; its layout is not described yet. */
struct OSAlarm;

/* 0x82e ticks of the 33.5 MHz / 64 timer: about 4 ms. */
#define EXTRA_KEYS_POLL_TICKS 0x82e

extern BOOL data_arm7_03807b38;
extern struct OSAlarm data_arm7_03807b3c;

u16 func_arm7_037fcfc0(void);
u16 func_arm7_037fd434(void);
void func_arm7_037fd424(struct OSAlarm *alarm);
u64 func_arm7_037fce88(void);
void func_arm7_037fd204(struct OSAlarm *alarm, u64 start, u64 period, void (*handler)(void *), void *arg);
void func_arm7_037fe41c(void *arg);

/* Starts the periodic alarm that copies the X/Y buttons and the lid state
 * for the ARM9. Returns FALSE when the tick or alarm system is not running
 * or the alarm is already set. */
BOOL func_arm7_037fe45c(void)
{
    if (!func_arm7_037fcfc0() || !func_arm7_037fd434()) {
        return FALSE;
    }
    if (data_arm7_03807b38) {
        return FALSE;
    }
    func_arm7_037fd424(&data_arm7_03807b3c);
    func_arm7_037fd204(&data_arm7_03807b3c, func_arm7_037fce88() + EXTRA_KEYS_POLL_TICKS, EXTRA_KEYS_POLL_TICKS, func_arm7_037fe41c, NULL);
    data_arm7_03807b38 = TRUE;
    return TRUE;
}
