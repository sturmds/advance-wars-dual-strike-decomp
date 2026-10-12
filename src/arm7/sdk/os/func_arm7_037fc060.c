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
 * Link-time constants of the ARM7 SDK build, tested at run time like ARM9's
 * SDK_SYS_STACKSIZE: both 0x400 in the ROM. tools/configure.py has no ARM7
 * absolute symbols yet, so this file does not link.
 */
extern char SYSMODE_STACK_BYTES_ARM7[];
extern char IRQ_STACK_BYTES_ARM7[];
/* Start of the ARM7 WRAM code; the stack's low end when SYS_STACKSIZE is 0. */
extern char func_arm7_037f8000[];

extern BOOL data_arm7_03807904;          /* set once threads are initialised */
extern OSThreadInfo data_arm7_0380790c;
extern OSThread *data_arm7_03807914;     /* OSThreadInfo.current, under its own symbol */
extern OSThread **gRunningThreadRef_Arm7;
extern OSThread data_arm7_038079d8;      /* the code that called this, made thread 0 */

ThreadSwapHook func_arm7_037fbba8(ThreadSwapHook callback);

/* ARM7 copy of OS_InitThread. Unlike ARM9's it creates no idle thread. */
void func_arm7_037fc060(void)
{
    u32 stackLo;
    int i;

    if (data_arm7_03807904) {
        return;
    }
    data_arm7_03807904 = TRUE;

    for (i = 0; i < THREAD_SLOT_COUNT; i++) {
        data_arm7_0380790c.threads[i] = NULL;
    }

    gRunningThreadRef_Arm7 = &data_arm7_03807914;
    data_arm7_038079d8.priority = OS_THREAD_LAUNCHER_PRIORITY;
    data_arm7_038079d8.id = 0;
    data_arm7_038079d8.state = OS_THREAD_STATE_READY;
    data_arm7_038079d8.next = NULL;
    data_arm7_038079d8.profiler = NULL;
    data_arm7_0380790c.threads[0] = &data_arm7_038079d8;
    data_arm7_0380790c.list = &data_arm7_038079d8;
    data_arm7_0380790c.current = &data_arm7_038079d8;

    if ((u32)SYSMODE_STACK_BYTES_ARM7 == 0) {
        stackLo = (u32)func_arm7_037f8000 - (u32)SYSMODE_STACK_BYTES_ARM7;
    } else {
        stackLo = ARM7_WRAM_STACKS_TOP - (u32)IRQ_STACK_BYTES_ARM7 - (u32)SYSMODE_STACK_BYTES_ARM7;
    }
    data_arm7_038079d8.stackBottom = ARM7_WRAM_STACKS_TOP - (u32)IRQ_STACK_BYTES_ARM7;
    data_arm7_038079d8.stackTop = stackLo;
    data_arm7_038079d8.stackWarningOffset = 0;
    *(u32 *)(data_arm7_038079d8.stackBottom - 4) = STACK_GUARD_LOW_MAGIC;
    *(u32 *)data_arm7_038079d8.stackTop = STACK_GUARD_HIGH_MAGIC;
    data_arm7_038079d8.joinQueue.head = 0;

    data_arm7_0380790c.maxEntry = THREAD_SLOT_COUNT;
    data_arm7_0380790c.isNeedRescheduling = FALSE;
    data_arm7_0380790c.inIrq = 0;
    THREAD_TABLE_PTR_ARM7 = &data_arm7_0380790c;
    func_arm7_037fbba8(NULL);
}
