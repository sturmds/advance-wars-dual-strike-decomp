#include "types.h"
#include "sdk/os_arm7.h"

extern OSThread** gRunningThreadRef_Arm7;

/*
 * ARM7 copy of OS_SleepThread: the current thread waits until it is woken.
 * With a queue, it adds its id to queue->head, a bit mask of thread ids.
 */
void Thread_WaitOnQueue_Arm7(OSThreadQueue* queue) {
  int irqState = Cpu_MaskIrq_Arm7();
  OSThread* current = *gRunningThreadRef_Arm7;

  if (queue != NULL) {
    queue->head |= (u16)(1 << current->id);
  }
  current->state = OS_THREAD_STATE_WAITING;
  Thread_SwitchToFirstReady_Arm7();
  Cpu_RestoreIrqBit_Arm7(irqState);
}
