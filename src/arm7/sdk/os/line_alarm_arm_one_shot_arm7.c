#include "types.h"
#include "sdk/os.h"
#include "sdk/gx.h"

int func_arm7_037fdb80(void);
void func_arm7_037fdb94(int state);
void func_arm7_037fdc10(void);
s32 func_arm7_037fd530(s32 vcount);
void LineAlarm_QueueByFrameLine_Arm7(OSVAlarm *alarm);

/*
 * The handler runs once, when VCOUNT reaches line, up to delay lines late:
 * this frame if the line is still ahead, otherwise the next. Stops the
 * system if the alarm is NULL or already set.
 */
void LineAlarm_ArmOneShot_Arm7(OSVAlarm *alarm, s16 line, s16 delay, OSVAlarmHandler handler, void *arg)
{
    int irqState = func_arm7_037fdb80();
    u16 vcount;
    s32 frame;

    if (alarm == NULL || alarm->handler != NULL) {
        func_arm7_037fdc10();
    }
    vcount = *REG_VCOUNT;
    frame = func_arm7_037fd530(vcount);
    alarm->period = FALSE;
    alarm->fire = line;
    if (line <= vcount) {
        frame++;
    }
    alarm->frame = frame;
    alarm->delay = delay;
    alarm->handler = handler;
    alarm->arg = arg;
    alarm->canceled = FALSE;
    LineAlarm_QueueByFrameLine_Arm7(alarm);
    func_arm7_037fdb94(irqState);
}
