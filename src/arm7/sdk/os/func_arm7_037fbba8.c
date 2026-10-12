#include "types.h"
#include "sdk/os_arm7.h"

extern OSThreadInfo data_arm7_0380790c;

ThreadSwapHook func_arm7_037fbba8(ThreadSwapHook callback) {
  int irqState = Cpu_MaskIrq_Arm7();
  ThreadSwapHook previous = data_arm7_0380790c.switchCallback;

  data_arm7_0380790c.switchCallback = callback;
  Cpu_RestoreIrqBit_Arm7(irqState);
  return previous;
}
