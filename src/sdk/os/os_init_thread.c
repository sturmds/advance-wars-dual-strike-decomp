#include "types.h"
#include "sdk/os.h"

#define STACK_GUARD_LOW_MAGIC 0xfddb597d
#define STACK_GUARD_HIGH_MAGIC 0x7bf9dd5b
#define THREAD_SLOT_COUNT 16
#define OS_THREAD_LAUNCHER_PRIORITY 16
#define OS_THREAD_PRIORITY_MAX 31
#define IDLE_THREAD_STACK_BYTES 200

/* Points at the scheduler state for the ARM7 and debuggers. */
#define THREAD_TABLE_PTR_ARM9 (*(OSThreadInfo **)0x027fffa0)

/*
 * Link-time constants of the SDK build. The ROM keeps testing them at run
 * time, so they must stay symbols: SYS_STACKSIZE 0, IRQ_STACKSIZE 0x400,
 * SECTION_ARENA_DTCM_START 0x027c12e0. The code matches, but this file
 * cannot link until the build defines them as absolute symbols.
 */
extern char SDK_SYS_STACKSIZE[];
extern char SDK_IRQ_STACKSIZE[];
extern char SDK_SECTION_ARENA_DTCM_START[];

extern u8 data_027c0000[];     /* DTCM base */
#define HW_DTCM_SVC_STACK ((u32)data_027c0000 + 0x3fc0)

extern BOOL OSi_IsThreadInitialized;
extern OSThread **data_022a9924;      /* current thread pointer */
extern OSThreadInfo data_022a9930;
extern OSThread *data_022a9938;       /* data_022a9930.current, under its own symbol */
extern OSThread OSi_LauncherThread;   /* the code that called this, made thread 0 */
extern OSThread data_022a9984;        /* the idle thread */
extern u32 data_022a9b74[];           /* end of the idle thread's stack */

void OSi_IdleThreadProc(void *arg);
void OS_CreateThread(OSThread *thread, void (*func)(void *), void *arg, void *stack, u32 stackSize, u32 priority);
ThreadSwapHook OS_SetSwitchThreadCallback(ThreadSwapHook callback);

void OS_InitThread(void)
{
    u32 stackLo;
    int i;

    if (OSi_IsThreadInitialized) {
        return;
    }
    OSi_IsThreadInitialized = TRUE;

    for (i = 0; i < THREAD_SLOT_COUNT; i++) {
        data_022a9930.threads[i] = NULL;
    }

    data_022a9924 = &data_022a9938;
    OSi_LauncherThread.priority = OS_THREAD_LAUNCHER_PRIORITY;
    OSi_LauncherThread.id = 0;
    OSi_LauncherThread.state = OS_THREAD_STATE_READY;
    OSi_LauncherThread.next = NULL;
    OSi_LauncherThread.profiler = NULL;
    data_022a9930.threads[0] = &OSi_LauncherThread;
    data_022a9930.list = &OSi_LauncherThread;
    data_022a9930.current = &OSi_LauncherThread;

    if ((u32)SDK_SYS_STACKSIZE == 0) {
        stackLo = (u32)SDK_SECTION_ARENA_DTCM_START - (u32)SDK_SYS_STACKSIZE;
    } else {
        stackLo = HW_DTCM_SVC_STACK - (u32)SDK_IRQ_STACKSIZE - (u32)SDK_SYS_STACKSIZE;
    }
    OSi_LauncherThread.stackBottom = HW_DTCM_SVC_STACK - (u32)SDK_IRQ_STACKSIZE;
    OSi_LauncherThread.stackTop = stackLo;
    OSi_LauncherThread.stackWarningOffset = 0;
    *(u32 *)(OSi_LauncherThread.stackBottom - 4) = STACK_GUARD_LOW_MAGIC;
    *(u32 *)OSi_LauncherThread.stackTop = STACK_GUARD_HIGH_MAGIC;
    OSi_LauncherThread.joinQueue.head = 0;

    data_022a9930.maxEntry = THREAD_SLOT_COUNT;
    data_022a9930.isNeedRescheduling = FALSE;
    data_022a9930.inIrq = 0;
    THREAD_TABLE_PTR_ARM9 = &data_022a9930;
    OS_SetSwitchThreadCallback(NULL);

    OS_CreateThread(&data_022a9984, OSi_IdleThreadProc, NULL, data_022a9b74, IDLE_THREAD_STACK_BYTES, OS_THREAD_PRIORITY_MAX);
    data_022a9984.priority = OS_THREAD_PRIORITY_MAX + 1;
    data_022a9984.state = OS_THREAD_STATE_READY;
}
