#include "types.h"
#include "sdk/os_arm7.h"

extern OSVAlarmQueue gOsiVAlarmQueue_Arm7;

/* Unlinks the alarm from the V-alarm queue. */
void LineAlarm_Unlink_Arm7(OSVAlarm* alarm) {
  OSVAlarm* prev;
  OSVAlarm* next;

  if (alarm == NULL) {
    return;
  }
  prev = alarm->prev;
  next = alarm->next;
  if (next != NULL) {
    next->prev = prev;
  } else {
    gOsiVAlarmQueue_Arm7.tail = prev;
  }
  if (prev != NULL) {
    prev->next = next;
  } else {
    gOsiVAlarmQueue_Arm7.head = next;
  }
}
