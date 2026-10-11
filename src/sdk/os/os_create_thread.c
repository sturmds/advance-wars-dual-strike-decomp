#include "types.h"
#include "sdk/os.h"

/* Marks written at both ends of a new thread's stack, to spot overflows. */
#define STACK_GUARD_LOW_MAGIC 0xfddb597d
#define STACK_GUARD_HIGH_MAGIC 0x7bf9dd5b

/* OSThreadInfo's table of threads by id, which follows its other fields. */
extern OSThread *data_022a9944[];

int OSi_GetUnusedThreadId(void);
void OSi_InsertThreadToList(OSThread *thread);
void MIi_CpuClear32(u32 value, void *dest, u32 size);
void OS_ExitThread(void);

void OS_CreateThread(OSThread *thread, void (*func)(void *), void *arg, void *stack, u32 stackSize, u32 priority)
{
    int irqState;
    s32 id;
    OSContext *context;
    volatile u32 zero; /* needed to match: the fill value goes through the stack */

    irqState = OS_DisableInterrupts();
    id = OSi_GetUnusedThreadId();
    if (id < 0) {
        OS_RestoreInterrupts(irqState);
        return;
    }
    thread->priority = priority;
    thread->id = id;
    thread->state = OS_THREAD_STATE_WAITING;
    thread->profiler = NULL;
    data_022a9944[id] = thread;
    OSi_InsertThreadToList(thread);

    thread->stackBottom = (u32)stack;
    thread->stackTop = (u32)stack - stackSize;
    thread->stackWarningOffset = 0;
    *(u32 *)(thread->stackBottom - 4) = STACK_GUARD_LOW_MAGIC;
    *(u32 *)thread->stackTop = STACK_GUARD_HIGH_MAGIC;
    thread->joinQueue.head = 0;

    context = &thread->context;
    OS_InitContext(context, (u32)func, (u32)stack - 4);
    context->r[0] = (u32)arg;
    context->lr = (u32)OS_ExitThread;

    zero = 0;
    MIi_CpuClear32(zero, (void *)((u32)stack - stackSize + 4), stackSize - 8);
    thread->mutex = NULL;
    thread->mutexQueue.head = NULL;
    thread->mutexQueue.tail = NULL;
    OS_RestoreInterrupts(irqState);
}
