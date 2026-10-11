#include "types.h"
#include "sdk/os.h"

extern OSThreadInfo OSi_ThreadInfo;
extern OSThread data_022a9984;

void OS_RescheduleThread(void);
void OSi_InsertThreadToList(OSThread *thread);

BOOL OS_SetThreadPriority(OSThread *thread, u32 priority)
{
    OSThread *cur;
    OSThread *prev;
    int irqState;
    prev = NULL;
    cur = OSi_ThreadInfo.list;
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
            OSi_ThreadInfo.list = thread->next;
        } else {
            prev->next = thread->next;
        }
        thread->priority = priority;
        OSi_InsertThreadToList(thread);
        OS_RescheduleThread();
    }
    OS_RestoreInterrupts(irqState);
    return TRUE;
}
