#include "types.h"
#include "sdk/os_arm7.h"

int func_arm7_037fdb80(void);
void func_arm7_037fdb94(int state);
void LineAlarm_Unlink_Arm7(OSVAlarm *alarm);

/*
 * Takes a V-count alarm out of the queue if it is set. Marks it canceled
 * either way, so a periodic alarm whose handler is running is not set again.
 */
void LineAlarm_Withdraw_Arm7(OSVAlarm *alarm)
{
    int irqState = func_arm7_037fdb80();

    alarm->canceled = TRUE;
    if (alarm->handler == NULL) {
        func_arm7_037fdb94(irqState);
        return;
    }
    LineAlarm_Unlink_Arm7(alarm);
    alarm->handler = NULL;
    func_arm7_037fdb94(irqState);
}
