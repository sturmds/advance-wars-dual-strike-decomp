#include "types.h"
#include "sdk/mb.h"
#include "sdk/os.h"

extern DlPlayWorkerArea *data_022acf00;

int func_0211dd18(void);
void MbTask_Set(void *queue, int unk_1, void (*task)(void), int unk_3);

/* Queues task for the task thread, if the thread was started. */
void MbTask_Post(void (*task)(void))
{
    OSIntrMode enabled = OS_DisableInterrupts();

    if (func_0211dd18()) {
        MbTask_Set(data_022acf00->queue, 0, task, 0);
    }
    OS_RestoreInterrupts(enabled);
}
