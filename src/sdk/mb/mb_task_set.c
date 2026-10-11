#include "types.h"
#include "sdk/mb.h"
#include "sdk/os.h"

extern DlPlayWorkerArea *data_022acf00;

int func_0211dd18(void);
int func_0210d354(OSThread *thread);
void OS_WakeupThreadDirect(OSThread *thread);

/* Queues info on the task thread's list, sorted by priority. The thread's own
 * end task goes last and stops further posts. A priority above
 * DLPLAY_JOB_PRIO_BOTTOM is taken relative to the task thread's. */
void MbTask_Set(DlPlayWorkerJob *info, void (*task)(DlPlayWorkerJob *info), void (*callback)(DlPlayWorkerJob *info),
               u32 priority)
{
    DlPlayWorkerArea *work = data_022acf00;
    int enabled;

    if (!func_0211dd18()) {
        OS_Terminate();
    }
    if (info->loading) {
        OS_Terminate();
    }
    if (priority > DLPLAY_JOB_PRIO_BOTTOM) {
        u32 current = func_0210d354(&work->thread);

        if (priority == DLPLAY_JOB_PRIO_HIGHER) {
            priority = current != 0 ? current - 1 : 0;
        } else if (priority == DLPLAY_JOB_PRIO_LOWER) {
            priority = current < DLPLAY_JOB_PRIO_BOTTOM ? current + 1 : DLPLAY_JOB_PRIO_BOTTOM;
        } else if (priority == DLPLAY_JOB_PRIO_SAME) {
            priority = current;
        } else {
            priority = DLPLAY_JOB_PRIO_BOTTOM;
        }
    }
    enabled = OS_DisableInterrupts();
    info->loading = 1;
    info->unk_04_1 = priority;
    info->task = task;
    info->callback = callback;
    if (MbTask_List(work) == NULL) {
        if ((u8 *)info == work->queue) {
            data_022acf00 = NULL;
        }
        MbTask_List(work) = info;
        OS_WakeupThreadDirect(&work->thread);
    } else {
        DlPlayWorkerJob *prev = MbTask_List(work);

        if ((u8 *)info == work->queue) {
            while (prev->next != NULL) {
                prev = prev->next;
            }
            prev->next = info;
            data_022acf00 = NULL;
        } else if (prev->unk_04_1 > priority) {
            MbTask_List(work) = info;
            info->next = prev;
        } else {
            DlPlayWorkerJob *next;

            while ((next = prev->next) != NULL && next->unk_04_1 <= priority) {
                prev = next;
            }
            info->next = next;
            prev->next = info;
        }
    }
    OS_RestoreInterrupts(enabled);
}
