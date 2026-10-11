#include "types.h"
#include "sdk/os.h"

extern OSThreadInfo data_022a9930;
extern OSThread data_022a9984;

void OS_RescheduleThread(void);
void func_0210d958(OSThread *thread);

BOOL OS_SetThreadPriority(OSThread *thread, u32 priority)
{
    OSThread *cur;
    OSThread *prev;
    int irqState;
    prev = NULL;
    cur = data_022a9930.list;
    irqState = OS_DisableInterrupts();

    while (cur != NULL && cur != thread) {
        prev = cur;
        cur = cur->next;
    }
    if (cur == NULL || cur == &data_022a9984) {
        OS_RestoreInterrupts(irqState);
        return FALSE;
    }
    if (cur->priority != priority) {
        if (prev == NULL) {
            data_022a9930.list = thread->next;
        } else {
            prev->next = thread->next;
        }
        thread->priority = priority;
        func_0210d958(thread);
        OS_RescheduleThread();
    }
    OS_RestoreInterrupts(irqState);
    return TRUE;
}
