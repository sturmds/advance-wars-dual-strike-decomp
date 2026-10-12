#include "types.h"
#include "sdk/os_arm7.h"

#define PROC_MODE_IRQ 0x12

extern u32 gSchedulerHoldDepth_Arm7;
extern OSThread** gRunningThreadRef_Arm7;
extern ThreadSwapHook
    data_arm7_03807908; /* called on every switch, before info->switchCallback */
extern OSThreadInfo data_arm7_0380790c;

/*
 * ARM7 copy of OS_RescheduleThread: switches to the first ready thread in
 * the list. Does nothing while the scheduler is disabled; in an interrupt it
 * only marks that a switch is needed.
 */
void Thread_SwitchToFirstReady_Arm7(void) {
  OSThread* current;
  OSThread* next;
  OSThreadInfo* info = &data_arm7_0380790c;

  if (gSchedulerHoldDepth_Arm7 != 0) {
    return;
  }
  if (info->inIrq != 0 || Cpu_CurrentMode_Arm7() == PROC_MODE_IRQ) {
    info->isNeedRescheduling = TRUE;
    return;
  }
  current = *gRunningThreadRef_Arm7;
  next = Thread_FirstRunnable_Arm7();
  if (current == next || next == NULL) {
    return;
  }
  if (Context_Capture_Arm7(&current->context) != 0) {
    return;
  }
  if (data_arm7_03807908 != NULL) {
    data_arm7_03807908(current, next);
  }
  if (info->switchCallback != NULL) {
    info->switchCallback(current, next);
  }
  data_arm7_0380790c.current = next;
  Context_Resume_Arm7(&next->context);
}
