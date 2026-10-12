#include "types.h"
#include "sdk/os_arm7.h"

#define STACK_GUARD_LOW_MAGIC 0xd73bfdf7
#define STACK_GUARD_HIGH_MAGIC 0xfbdd37bb
#define THREAD_SLOT_COUNT 16
#define OS_THREAD_LAUNCHER_PRIORITY 16

/* Top of the ARM7 system stack in its private WRAM. */
#define ARM7_WRAM_STACKS_TOP 0x0380ff80

/* Points at the ARM7 scheduler state, next to ARM9's at 0x027fffa0. */
#define THREAD_TABLE_PTR_ARM7 (*(OSThreadInfo **)0x027fffa4)

/*
 * Link-time constants of the ARM7 SDK build, tested at run time: both 0x400.
 * tools/configure.py defines them for the ARM7 link (ARM7_ABSOLUTE_SYMBOLS).
 */
extern char SDK_SYS_STACKSIZE[];
extern char SDK_IRQ_STACKSIZE[];
/*
 * Start of the ARM7 WRAM code, used as the stack's low end when
 * SYS_STACKSIZE is 0. Only the address matters: the function there is the
 * first object in WRAM.
 */
void VBlankIntr_Arm7(void);

extern BOOL gThreadTableReady_Arm7;
extern OSThreadInfo data_arm7_0380790c;
extern OSThread *data_arm7_03807914;     /* OSThreadInfo.current, under its own symbol */
extern OSThread **gRunningThreadRef_Arm7;
extern OSThread gBootThread_Arm7;      /* the code that called this, made thread 0 */

ThreadSwapHook func_arm7_037fbba8(ThreadSwapHook callback);

/* ARM7 copy of OS_InitThread. Unlike ARM9's it creates no idle thread. */
void Thread_SetUpTableOnce_Arm7(void)
{
    u32 stackLo;
    int i;

    if (gThreadTableReady_Arm7) {
        return;
    }
    gThreadTableReady_Arm7 = TRUE;

    for (i = 0; i < THREAD_SLOT_COUNT; i++) {
        data_arm7_0380790c.threads[i] = NULL;
    }

    gRunningThreadRef_Arm7 = &data_arm7_03807914;
    gBootThread_Arm7.priority = OS_THREAD_LAUNCHER_PRIORITY;
    gBootThread_Arm7.id = 0;
    gBootThread_Arm7.state = OS_THREAD_STATE_READY;
    gBootThread_Arm7.next = NULL;
    gBootThread_Arm7.profiler = NULL;
    data_arm7_0380790c.threads[0] = &gBootThread_Arm7;
    data_arm7_0380790c.list = &gBootThread_Arm7;
    data_arm7_0380790c.current = &gBootThread_Arm7;

    if ((u32)SDK_SYS_STACKSIZE == 0) {
        stackLo = (u32)VBlankIntr_Arm7 - (u32)SDK_SYS_STACKSIZE;
    } else {
        stackLo = ARM7_WRAM_STACKS_TOP - (u32)SDK_IRQ_STACKSIZE - (u32)SDK_SYS_STACKSIZE;
    }
    gBootThread_Arm7.stackBottom = ARM7_WRAM_STACKS_TOP - (u32)SDK_IRQ_STACKSIZE;
    gBootThread_Arm7.stackTop = stackLo;
    gBootThread_Arm7.stackWarningOffset = 0;
    *(u32 *)(gBootThread_Arm7.stackBottom - 4) = STACK_GUARD_LOW_MAGIC;
    *(u32 *)gBootThread_Arm7.stackTop = STACK_GUARD_HIGH_MAGIC;
    gBootThread_Arm7.joinQueue.head = 0;

    data_arm7_0380790c.maxEntry = THREAD_SLOT_COUNT;
    data_arm7_0380790c.isNeedRescheduling = FALSE;
    data_arm7_0380790c.inIrq = 0;
    THREAD_TABLE_PTR_ARM7 = &data_arm7_0380790c;
    func_arm7_037fbba8(NULL);
}
