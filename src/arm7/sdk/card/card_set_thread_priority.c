/* needed to match: the original was built without copy propagation */
#pragma opt_propagation off
#include "types.h"
#include "sdk/card_arm7.h"
#include "sdk/os_arm7.h"

/* Sets the card thread's priority and returns the old one. */
u32 CARD_SetThreadPriority(u32 priority) {
  CARDiArm7Common* common = &data_arm7_03809280;
  int interrupts = Cpu_MaskIrq_Arm7();
  u32 old = common->priority;

  common->priority = priority;
  Thread_Reprioritize_Arm7(&common->thread, common->priority);
  Cpu_RestoreIrqBit_Arm7(interrupts);
  return old;
}
