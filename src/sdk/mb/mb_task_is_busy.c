#include "types.h"
#include "sdk/mb.h"

/* 1 while the task is queued or running on the task thread. */
int MbTask_IsBusy(DlPlayWorkerJob* task) { return task->busy ? 1 : 0; }
