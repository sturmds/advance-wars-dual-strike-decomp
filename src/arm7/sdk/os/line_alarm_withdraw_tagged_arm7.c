#include "types.h"
#include "sdk/os.h"

extern OSVAlarmQueue gOsiVAlarmQueue_Arm7;
int func_arm7_037fdb80(void);
void func_arm7_037fdb94(int state);
void func_arm7_037fdc10(void);
void LineAlarm_Withdraw_Arm7(OSVAlarm *alarm);

/* Cancels every set V-count alarm with this tag. Stops the system if tag is 0. */
void LineAlarm_WithdrawTagged_Arm7(u32 tag)
{
    int irqState = func_arm7_037fdb80();
    OSVAlarm *alarm;
    OSVAlarm *next;

    if (tag == 0) {
        func_arm7_037fdc10();
    }
    for (alarm = gOsiVAlarmQueue_Arm7.head, next = alarm != NULL ? alarm->next : NULL; alarm != NULL;
         alarm = next, next = next != NULL ? next->next : NULL) {
        if (alarm->tag == tag) {
            LineAlarm_Withdraw_Arm7(alarm);
        }
    }
    func_arm7_037fdb94(irqState);
}
