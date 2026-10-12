#include "types.h"
#include "sdk/os.h"

void OS_RescheduleThread(void);

void OS_WakeupThreadDirect(OSThread* thread) {
  int irqState = OS_DisableInterrupts();

  thread->state = OS_THREAD_STATE_READY;
  OS_RescheduleThread();
  OS_RestoreInterrupts(irqState);
}
