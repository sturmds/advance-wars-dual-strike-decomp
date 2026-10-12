#include "types.h"
#include "sdk/os_arm7.h"

/* System clock cycles per millisecond; one OS tick is 64 cycles. */
#define BUS_CLOCKS_PER_MS 33514

/* Points at OSThreadInfo.current. */
extern OSThread **gRunningThreadRef_Arm7;

void func_arm7_037fd424(OSAlarm *alarm);
void Alarm_ArmOneShot_Arm7(OSAlarm *alarm, OSTick delay, OSAlarmHandler handler, void *arg);

/* ARM7 copy of OS_Sleep: the current thread sleeps for msec milliseconds. */
void Thread_NapMilliseconds_Arm7(u32 msec)
{
    OSAlarm alarm;

    func_arm7_037fd424(&alarm);
    Alarm_ArmOneShot_Arm7(&alarm, (msec * BUS_CLOCKS_PER_MS) >> 6, Thread_WakeAfterNap_Arm7, *gRunningThreadRef_Arm7);
    Thread_WaitOnQueue_Arm7(NULL);
}
