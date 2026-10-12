#include "types.h"
#include "sdk/mb.h"
#include "sdk/os.h"

void OS_SleepThread(OSThreadQueue* queue);
void OS_ExitThread(void);

/* The MB task thread. It sleeps at priority 0 while the list is empty, runs
 * each task at the task's priority, then its callback, and exits after the end
 * task. */
void MbTask_ThreadMain(void* arg) {
  DlPlayWorkerArea* work = arg;

  for (;;) {
    int enabled = OS_DisableInterrupts();
    DlPlayWorkerJob* info;
    u32 current;
    u32 next;
    int enabledAfter; /* needed to match: not the same variable as enabled */
    void (*callback)(DlPlayWorkerJob* info);

    while (work->list == NULL) {
      OS_SetThreadPriority(&work->thread, 0);
      OS_SleepThread(NULL);
    }
    info = work->list;
    work->list = work->list->next;
    OS_SetThreadPriority(&work->thread, info->priority);
    OS_RestoreInterrupts(enabled);

    if (info->task != NULL) {
      info->task(info);
    }

    enabledAfter = OS_DisableInterrupts();
    callback = info->callback;
    current = OS_GetThreadPriority(&work->thread);
    if (work->list == NULL) {
      next = 0;
    } else if (current < work->list->priority) {
      next = work->list->priority;
    } else {
      next = current;
    }
    if (next != current) {
      OS_SetThreadPriority(&work->thread, next);
    }
    info->next = NULL;
    info->busy = 0;
    if (callback != NULL) {
      callback(info);
    }
    if (info == &work->endTask) {
      break;
    }
    OS_RestoreInterrupts(enabledAfter);
  }
  OS_ExitThread();
}
