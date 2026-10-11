#include "types.h"
#include "sdk/os.h"

extern OSThread **data_022a9924;
extern OSThread *data_022a9944[];

void OS_RescheduleThread(void);
void OS_WakeupThread(OSThreadQueue *queue);
void OSi_RemoveThreadFromList(OSThread *thread);
void func_0210df74(OSThread *thread);

void OS_ExitThread(void)
{
    OSThread *current;

    OS_DisableInterrupts();
    current = *data_022a9924;
    func_0210df74(current);
    OSi_RemoveThreadFromList(current);
    data_022a9944[current->id] = NULL;
    current->state = OS_THREAD_STATE_TERMINATED;
    OS_WakeupThread(&current->joinQueue);
    OS_RescheduleThread();
    OS_Terminate();
}
