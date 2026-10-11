#ifndef SDK_OS_H
#define SDK_OS_H

#include "types.h"

typedef volatile u16 vu16;
typedef volatile u32 vu32;

/* EXMEMCNT: external memory bus control. */
#define REG_EXMEMCNT (*(vu16 *)0x04000204)

/* Threads waiting on something, as a pair of thread ids. */
typedef struct OSThreadQueue {
    u16 head;
    u16 tail;
} OSThreadQueue;

/* Disables interrupts and halts the CPU for good. */
void OS_Terminate(void);

/* Turn interrupts off and return the previous state; hand it back to restore. */
int OS_DisableInterrupts(void);
void OS_RestoreInterrupts(int state);

#endif
