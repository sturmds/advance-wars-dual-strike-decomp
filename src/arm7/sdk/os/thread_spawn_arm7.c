#include "types.h"
#include "sdk/os_arm7.h"

/* Marks written at both ends of a new thread's stack, to spot overflows (not ARM9's values). */
#define STACK_GUARD_LOW_MAGIC 0xd73bfdf7
#define STACK_GUARD_HIGH_MAGIC 0xfbdd37bb

extern OSThread *data_arm7_03807920[];   /* OSThreadInfo.threads, under its own symbol */

int func_arm7_037fdb80(void);
void func_arm7_037fdb94(int state);
int func_arm7_037fc23c(void);
void func_arm7_037fc1d4(OSThread *thread);
void func_arm7_037fc270(OSContext *context, u32 pc, u32 sp);
void func_arm7_037fde5c(u32 data, void *dest, int size);
void Thread_EndCurrent_Arm7(void);

/*
 * ARM7 copy of OS_CreateThread. stack is the top of the stack (it grows
 * down). The thread starts waiting; it returns into Thread_EndCurrent_Arm7.
 */
void Thread_Spawn_Arm7(OSThread *thread, void (*func)(void *), void *arg, void *stack, u32 stackSize, u32 priority)
{
    int irqState;
    s32 id;
    OSContext *context;
    volatile u32 zero; /* needed to match: the fill value goes through the stack */

    irqState = func_arm7_037fdb80();
    id = func_arm7_037fc23c();
    if (id < 0) {
        func_arm7_037fdb94(irqState);
        return;
    }
    thread->priority = priority;
    thread->id = id;
    thread->state = OS_THREAD_STATE_WAITING;
    thread->profiler = NULL;
    data_arm7_03807920[id] = thread;
    func_arm7_037fc1d4(thread);

    thread->stackBottom = (u32)stack;
    thread->stackTop = (u32)stack - stackSize;
    thread->stackWarningOffset = 0;
    *(u32 *)(thread->stackBottom - 4) = STACK_GUARD_LOW_MAGIC;
    *(u32 *)thread->stackTop = STACK_GUARD_HIGH_MAGIC;
    thread->joinQueue.head = 0;

    context = &thread->context;
    func_arm7_037fc270(context, (u32)func, (u32)stack - 4);
    context->r[0] = (u32)arg;
    context->lr = (u32)Thread_EndCurrent_Arm7;

    zero = 0;
    func_arm7_037fde5c(zero, (void *)((u32)stack - stackSize + 4), stackSize - 8);
    thread->mutex = NULL;
    thread->mutexQueue.head = NULL;
    thread->mutexQueue.tail = NULL;
    func_arm7_037fdb94(irqState);
}
