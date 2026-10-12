#include "types.h"
#include "sdk/os_arm7.h"

extern OSVAlarmQueue gOsiVAlarmQueue_Arm7;

/* Cancels every set V-count alarm with this tag. Stops the system if tag is 0. */
void LineAlarm_WithdrawTagged_Arm7(u32 tag) {
  int irqState = Cpu_MaskIrq_Arm7();
  OSVAlarm* alarm;
  OSVAlarm* next;

  if (tag == 0) {
    Cpu_HaltForever_Arm7();
  }
  for (alarm = gOsiVAlarmQueue_Arm7.head,
      next = alarm != NULL ? alarm->next : NULL;
       alarm != NULL; alarm = next, next = next != NULL ? next->next : NULL) {
    if (alarm->tag == tag) {
      LineAlarm_Withdraw_Arm7(alarm);
    }
  }
  Cpu_RestoreIrqBit_Arm7(irqState);
}
