#include "types.h"
#include "sdk/os_arm7.h"
#include "sdk/gx.h"

extern OSVAlarmQueue gOsiVAlarmQueue_Arm7;
extern s32 data_arm7_03807aa0;
s32 func_arm7_037fd530(s32 vcount);

/*
 * V-count interrupt: runs every alarm that is due, moves missed ones to the
 * next frame, and points the V-count interrupt at the next alarm.
 */
void LineAlarm_OnVcountIrq_Arm7(void) {
  OSVAlarm* alarm;
  u16 dispstat;

  Irq_ClearEnableBits_Arm7(IRQ_BIT_LINE_MATCH);
  *REG_DISPSTAT &= ~DISPSTAT_VCOUNT_IRQ;
  OS7_IRQ_CHECK_FLAGS |= IRQ_BIT_LINE_MATCH;
  /* the line this interrupt was set for, minus one */
  dispstat = *REG_DISPSTAT;
  func_arm7_037fd530((((dispstat >> 8) & 0xff) | ((dispstat << 1) & 0x100)) -
                     1);

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
        Irq_ClearEnableBits_Arm7(IRQ_BIT_LINE_MATCH);
        *REG_DISPSTAT &= ~DISPSTAT_VCOUNT_IRQ;
        Irq_AckPending_Arm7(IRQ_BIT_LINE_MATCH);
        /* fall through */
      case LINE_ALARM_ON_TIME:
        handler = alarm->handler;
        LineAlarm_Unlink_Arm7(alarm);
        alarm->handler = NULL;
        if (handler != NULL) {
          handler(alarm->arg);
        }
        if (alarm->period && !alarm->canceled) {
          alarm->handler = handler;
          alarm->frame = data_arm7_03807aa0 + 1;
          LineAlarm_PushTail_Arm7(alarm);
        }
        break;
      case LINE_ALARM_TOO_LATE:
        LineAlarm_Unlink_Arm7(alarm);
        LineAlarm_PushTail_Arm7(alarm);
        alarm->frame = data_arm7_03807aa0 + 1;
        break;
    }
  }
}
