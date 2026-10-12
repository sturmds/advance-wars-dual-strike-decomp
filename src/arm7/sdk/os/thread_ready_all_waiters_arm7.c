#include "types.h"
#include "sdk/os_arm7.h"

extern OSThreadInfo data_arm7_0380790c;

/*
 * ARM7 copy of OS_WakeupThread: makes every thread waiting on queue ready.
 * queue->head is a bit mask of the waiting thread ids.
 */
void Thread_ReadyAllWaiters_Arm7(OSThreadQueue* queue) {
  s32 waiting;
  int irqState;
  OSThread* thread;

  irqState = Cpu_MaskIrq_Arm7();
  waiting = queue->head;
  if (waiting != 0) {
    for (thread = data_arm7_0380790c.list; thread != NULL;
         thread = thread->next) {
      if (waiting & (1 << thread->id)) {
        thread->state = OS_THREAD_STATE_READY;
      }
    }
    queue->head = 0;
    Thread_SwitchToFirstReady_Arm7();
  }
  Cpu_RestoreIrqBit_Arm7(irqState);
}
