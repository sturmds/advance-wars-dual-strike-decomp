#include "types.h"
#include "sdk/mb.h"
#include "sdk/os.h"

void OS_WakeupThreadDirect(OSThread* thread);

/* Queues info on the task thread's list, sorted by priority. The thread's own
 * end task goes last and stops further posts. A priority above
 * DLPLAY_JOB_PRIO_BOTTOM is taken relative to the task thread's. */
void MbTask_Set(DlPlayWorkerJob* info, void (*task)(DlPlayWorkerJob* info),
                void (*callback)(DlPlayWorkerJob* info), u32 priority) {
  DlPlayWorkerArea* work = data_022acf00;
  int enabled;

  if (!MbTask_IsAvailable()) {
    OS_Terminate();
  }
  if (info->busy) {
    OS_Terminate();
  }
  if (priority > DLPLAY_JOB_PRIO_BOTTOM) {
    u32 current = OS_GetThreadPriority(&work->thread);

    if (priority == DLPLAY_JOB_PRIO_HIGHER) {
      priority = current != 0 ? current - 1 : 0;
    } else if (priority == DLPLAY_JOB_PRIO_LOWER) {
      priority = current < DLPLAY_JOB_PRIO_BOTTOM ? current + 1
                                                  : DLPLAY_JOB_PRIO_BOTTOM;
    } else if (priority == DLPLAY_JOB_PRIO_SAME) {
      priority = current;
    } else {
      priority = DLPLAY_JOB_PRIO_BOTTOM;
    }
  }
  enabled = OS_DisableInterrupts();
  info->busy = 1;
  info->priority = priority;
  info->task = task;
  info->callback = callback;
  if (work->list == NULL) {
    if (info == &work->endTask) {
      data_022acf00 = NULL;
    }
    work->list = info;
    OS_WakeupThreadDirect(&work->thread);
  } else {
    DlPlayWorkerJob* prev = work->list;

    if (info == &work->endTask) {
      while (prev->next != NULL) {
        prev = prev->next;
      }
      prev->next = info;
      data_022acf00 = NULL;
    } else if (prev->priority > priority) {
      work->list = info;
      info->next = prev;
    } else {
      DlPlayWorkerJob* next;

      while ((next = prev->next) != NULL && next->priority <= priority) {
        prev = next;
      }
      info->next = next;
      prev->next = info;
    }
  }
  OS_RestoreInterrupts(enabled);
}
