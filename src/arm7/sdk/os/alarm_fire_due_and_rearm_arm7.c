#include "types.h"
#include "sdk/os_arm7.h"

extern AlarmList gOsiAlarmQueue_Arm7;

/*
 * Runs on timer 1: stops the timer, fires the first alarm if it is due,
 * puts a periodic alarm back in the queue, then starts the timer for the
 * next one.
 */
void Alarm_FireDueAndRearm_Arm7(void) {
  OSTick now;
  OSAlarm* alarm;
  OSAlarm* next;
  OSAlarmHandler handler;

  REG_TM1CNT_H = 0;
  Irq_ClearEnableBits_Arm7(IRQ_MASK_TIMER1);
  OS7_IRQ_CHECK_FLAGS |= IRQ_MASK_TIMER1;
  now = Tick_Read64_Arm7();

  alarm = gOsiAlarmQueue_Arm7.head;
  if (alarm == NULL) {
    return;
  }
  if (now < alarm->fire) {
    Alarm_StartTimer1ForDue_Arm7(alarm);
    return;
  }

  next = alarm->next;
  gOsiAlarmQueue_Arm7.head = next;
  if (next == NULL) {
    gOsiAlarmQueue_Arm7.tail = NULL;
  } else {
    next->prev = NULL;
  }

  handler = alarm->handler;
  if (alarm->period == 0) {
    alarm->handler = NULL;
  }
  if (handler != NULL) {
    handler(alarm->arg);
  }
  if (alarm->period != 0) {
    alarm->handler = handler;
    Alarm_QueueByFireTime_Arm7(alarm, 0);
  }

  if (gOsiAlarmQueue_Arm7.head != NULL) {
    Alarm_StartTimer1ForDue_Arm7(gOsiAlarmQueue_Arm7.head);
  }
}
