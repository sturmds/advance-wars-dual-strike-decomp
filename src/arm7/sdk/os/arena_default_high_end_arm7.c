#include "types.h"
#include "sdk/os_arm7.h"

/*
 * Link-time constants of the ARM7 SDK build, both 0x400 here. The ROM keeps
 * testing them at run time, so they must stay symbols. ARM7_ABSOLUTE_SYMBOLS
 * in tools/configure.py gives them their values in the linker script.
 */
extern char SDK_SYS_STACKSIZE[];
extern char SDK_IRQ_STACKSIZE[];

#define MAIN_SUBPRIV_ARENA_HI 0x027ff000
#define ARM7_OWN_WRAM_BASE 0x03800000          /* ARM7's own WRAM starts here; shared WRAM ends here */
#define ARM7_WRAM_IRQ_STACK_CEIL 0x0380ff80

/* ARM7 copy of OS_GetInitArenaHi. Initial high end of each arena; NULL for an id with no arena. */
void *Arena_DefaultHighEnd_Arm7(OSArenaId id)
{
    u32 irqStackLo;

    switch (id) {
    case OS_ARENA_MAIN_SUBPRIV:
        return (void *)MAIN_SUBPRIV_ARENA_HI;
    case OS_ARENA_WRAM_SUB:
        return (void *)ARM7_OWN_WRAM_BASE;
    case OS_ARENA_WRAM_SUBPRIV:
        irqStackLo = ARM7_WRAM_IRQ_STACK_CEIL - (u32)SDK_IRQ_STACKSIZE;
        if ((u32)SDK_SYS_STACKSIZE == 0) {
            return (void *)ARM7_OWN_WRAM_BASE;
        }
        if ((s32)SDK_SYS_STACKSIZE < 0) {
            return (void *)(ARM7_OWN_WRAM_BASE - (u32)SDK_SYS_STACKSIZE);
        }
        return (void *)(irqStackLo - (u32)SDK_SYS_STACKSIZE);
    default:
        return NULL;
    }
}
