#include "types.h"
#include "sdk/mb.h"
#include "sdk/os.h"

void OS_CreateThread(OSThread* thread, void (*proc)(void*), void* arg,
                     void* stack, u32 stackSize, u32 priority);
void OS_WakeupThreadDirect(OSThread* thread);
void MbTask_ThreadMain(void* arg);

/* Starts the task thread in work, which is size bytes long: the OSThread at
 * the start, the task queue, then the stack. Does nothing when a task thread
 * already runs. */
void MbTask_Start(DlPlayWorkerArea* work, u32 size) {
  OSIntrMode enabled = OS_DisableInterrupts();

  if (data_022acf00 == NULL) {
    u32 stackSize;

    data_022acf00 = work;
    MbTask_InitInfo(&work->endTask);
    work->list = NULL;
    stackSize = (size - DLPLAY_WORKER_STACK_AT) & ~3;
    OS_CreateThread((OSThread*)work, MbTask_ThreadMain, work,
                    work->stack + stackSize, stackSize, 0);
    OS_WakeupThreadDirect((OSThread*)work);
  }
  OS_RestoreInterrupts(enabled);
}
