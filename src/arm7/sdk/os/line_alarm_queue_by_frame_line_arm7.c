#include "types.h"
#include "sdk/os_arm7.h"

extern OSVAlarmQueue gOsiVAlarmQueue_Arm7;

/*
 * Puts the alarm in the V-alarm queue by frame, then line, after alarms that
 * fire at the same time. A new head gets the interrupt.
 */
void LineAlarm_QueueByFrameLine_Arm7(OSVAlarm* alarm) {
  OSVAlarm* target;

  for (target = gOsiVAlarmQueue_Arm7.head; target != NULL;
       target = target->next) {
    OSVAlarm* prev;

    if (target->frame < alarm->frame) {
      continue;
    }
    if (target->frame == alarm->frame && target->fire <= alarm->fire) {
      continue;
    }
    prev = target->prev;
    alarm->prev = prev;
    alarm->next = target;
    target->prev = alarm;
    if (prev != NULL) {
      prev->next = alarm;
    } else {
      gOsiVAlarmQueue_Arm7.head = alarm;
      LineAlarm_ProgramVcountIrq_Arm7(alarm);
    }
    return;
  }
  LineAlarm_PushTail_Arm7(alarm);
}
