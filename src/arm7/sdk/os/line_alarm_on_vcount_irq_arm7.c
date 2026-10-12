#include "types.h"
#include "sdk/os.h"
#include "sdk/gx.h"

extern OSVAlarmQueue gOsiVAlarmQueue_Arm7;
extern s32 data_arm7_03807aa0;
OSIrqMask func_arm7_037fb620(OSIrqMask mask);
OSIrqMask func_arm7_037fb654(OSIrqMask mask);
s32 func_arm7_037fd530(s32 vcount);
int LineAlarm_ClassifyTiming_Arm7(OSVAlarm *alarm, s32 frame, s32 vcount);
void LineAlarm_ProgramVcountIrq_Arm7(OSVAlarm *alarm);
void func_arm7_037fd9f0(OSVAlarm *alarm);
void func_arm7_037fda28(OSVAlarm *alarm);

/*
 * V-count interrupt: runs every alarm that is due, moves missed ones to the
 * next frame, and points the V-count interrupt at the next alarm.
 */
void LineAlarm_OnVcountIrq_Arm7(void)
{
    OSVAlarm *alarm;
    u16 dispstat;

    func_arm7_037fb654(IRQ_BIT_LINE_MATCH);
    *REG_DISPSTAT &= ~DISPSTAT_VCOUNT_IRQ;
    OS7_IRQ_CHECK_FLAGS |= IRQ_BIT_LINE_MATCH;
    /* the line this interrupt was set for, minus one */
    dispstat = *REG_DISPSTAT;
    func_arm7_037fd530((((dispstat >> 8) & 0xff) | ((dispstat << 1) & 0x100)) - 1);

    while ((alarm = gOsiVAlarmQueue_Arm7.head) != NULL) {
        u16 vcount = *REG_VCOUNT;
        s32 frame = func_arm7_037fd530(vcount);
        OSVAlarmHandler handler;

        switch (LineAlarm_ClassifyTiming_Arm7(alarm, frame, vcount)) {
        case LINE_ALARM_EARLY:
            LineAlarm_ProgramVcountIrq_Arm7(alarm);
            vcount = *REG_VCOUNT;
            if (alarm->fire != vcount || alarm->frame != frame) {
                return;
            }
            /* the line came while the interrupt was being set: run it now */
            func_arm7_037fb654(IRQ_BIT_LINE_MATCH);
            *REG_DISPSTAT &= ~DISPSTAT_VCOUNT_IRQ;
            func_arm7_037fb620(IRQ_BIT_LINE_MATCH);
            /* fall through */
        case LINE_ALARM_ON_TIME:
            handler = alarm->handler;
            func_arm7_037fd9f0(alarm);
            alarm->handler = NULL;
            if (handler != NULL) {
                handler(alarm->arg);
            }
            if (alarm->period && !alarm->canceled) {
                alarm->handler = handler;
                alarm->frame = data_arm7_03807aa0 + 1;
                func_arm7_037fda28(alarm);
            }
            break;
        case LINE_ALARM_TOO_LATE:
            func_arm7_037fd9f0(alarm);
            func_arm7_037fda28(alarm);
            alarm->frame = data_arm7_03807aa0 + 1;
            break;
        }
    }
}
