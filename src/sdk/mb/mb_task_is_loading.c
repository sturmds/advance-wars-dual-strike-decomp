#include "types.h"
#include "sdk/mb.h"

/* 1 while the task thread has a page load queued or running. */
int MbTask_IsLoading(DlPlayWorkerJob *task)
{
    return task->loading ? 1 : 0;
}
