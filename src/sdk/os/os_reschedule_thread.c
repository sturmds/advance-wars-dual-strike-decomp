#include "types.h"
#include "sdk/os.h"

#define PROC_MODE_IRQ 0x12

extern u32 data_022a9920;
extern OSThread **data_022a9924;
extern ThreadSwapHook data_022a992c;
extern OSThreadInfo data_022a9930;

u32 func_0210eb64(void);
OSThread *Thread_FirstRunnable(void);
int func_0210da54(OSThread *thread);
void func_0210daa0(OSThread *thread);

void OS_RescheduleThread(void)
{
    OSThread *current;
    OSThread *next;
    OSThreadInfo *info = &data_022a9930;

    if (data_022a9920 != 0) {
        return;
    }
    if (info->inIrq != 0 || func_0210eb64() == PROC_MODE_IRQ) {
        info->isNeedRescheduling = TRUE;
        return;
    }
    current = *data_022a9924;
    next = Thread_FirstRunnable();
    if (current == next || next == NULL) {
        return;
    }
    if (func_0210da54(current) != 0) {
        return;
    }
    if (data_022a992c != NULL) {
        data_022a992c(current, next);
    }
    if (info->switchCallback != NULL) {
        info->switchCallback(current, next);
    }
    data_022a9930.current = next;
    func_0210daa0(next);
}
