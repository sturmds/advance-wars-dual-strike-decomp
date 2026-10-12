#include "types.h"
#include "sdk/os_arm7.h"
#include "sdk/wmsp.h"

extern OSThread data_arm7_0380749c;
extern OSThread data_arm7_03807424;

int func_arm7_027c7aa0(void);

void Wmsp_SetAltThreadPriorities(void) {
  int irqState = Cpu_MaskIrq_Arm7();

  Thread_HoldSwitching_Arm7();
  Thread_Reprioritize_Arm7(&data_arm7_0380749c,
                           data_arm7_027d7c44.threadPriority[0][0]);
  Thread_Reprioritize_Arm7((OSThread*)func_arm7_027c7aa0(),
                           data_arm7_027d7c44.threadPriority[0][1]);
  Thread_Reprioritize_Arm7(&data_arm7_03807424,
                           data_arm7_027d7c44.threadPriority[0][2]);
  Thread_ReleaseSwitching_Arm7();
  Cpu_RestoreIrqBit_Arm7(irqState);
}
