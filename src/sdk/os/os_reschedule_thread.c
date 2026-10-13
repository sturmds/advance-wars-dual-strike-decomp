#include "types.h"
#include "sdk/os.h"

#define PROC_MODE_IRQ 0x12

OSThread** data_022a9924;
static u32 data_022a9920;
extern ThreadSwapHook data_022a992c;

u32 OS_GetProcMode(void);
OSThread* Thread_FirstRunnable(void);
int OS_SaveContext(OSThread* thread);
void OS_LoadContext(OSThread* thread);

void OS_RescheduleThread(void) {
  OSThread* current;
  OSThread* next;
  OSThreadInfo* info = &OSi_ThreadInfo;

  if (data_022a9920 != 0) {
    return;
  }
  if (info->inIrq != 0 || OS_GetProcMode() == PROC_MODE_IRQ) {
    info->isNeedRescheduling = TRUE;
    return;
  }
  current = *data_022a9924;
  next = Thread_FirstRunnable();
  if (current == next || next == NULL) {
    return;
  }
  if (OS_SaveContext(current) != 0) {
    return;
  }
  if (data_022a992c != NULL) {
    data_022a992c(current, next);
  }
  if (info->switchCallback != NULL) {
    info->switchCallback(current, next);
  }
  OSi_ThreadInfo.current = next;
  OS_LoadContext(next);
}
