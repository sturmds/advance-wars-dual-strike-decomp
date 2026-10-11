#ifndef SDK_OS_H
#define SDK_OS_H

#include "types.h"

#define REG_IME ((volatile u16 *)0x04000208)
#define REG_IE ((volatile u32 *)0x04000210)
#define REG_IF ((volatile u32 *)0x04000214)

typedef u32 OSIrqMask;
typedef void (*OSIrqFunction)(void);

/* One entry of the handler table that runs timer and DMA interrupts. */
typedef struct IrqSlotEntry {
    void (*handler)(void *);       /* cleared after it runs once */
    BOOL keepEnabled;              /* when 0, the interrupt is disabled after it fires */
    void *arg;
} IrqSlotEntry;

extern IrqSlotEntry gOsIrqSlots[];

typedef volatile u16 vu16;
typedef volatile u32 vu32;

/* EXMEMCNT: external memory bus control. */
#define REG_EXMEMCNT (*(vu16 *)0x04000204)

typedef struct OSThread OSThread;

/* Threads waiting on something, as a pair of thread ids. */
typedef struct OSThreadQueue {
    u16 head;
    u16 tail;
} OSThreadQueue;

enum {
    OS_THREAD_STATE_WAITING = 0,
    OS_THREAD_STATE_READY = 1,
    OS_THREAD_STATE_TERMINATED = 2
};

struct OSThread {
    u8 context[0x64];
    u32 state;
    OSThread *next;
    u32 id;
    u32 priority;
    u8 pad_74[0x1c];
    OSThreadQueue joinQueue;
};

typedef void (*ThreadSwapHook)(OSThread *from, OSThread *to);

/* Scheduler state. The thread list is sorted by priority, lowest value first. */
typedef struct OSThreadInfo {
    u16 isNeedRescheduling;
    u16 pad_2;
    u16 inIrq;
    u16 pad_6;
    OSThread *current;
    OSThread *list;
    ThreadSwapHook switchCallback;
} OSThreadInfo;

/* Disables interrupts and halts the CPU for good. */
void OS_Terminate(void);

/* Each of these returns the previous value of IE. */
OSIrqMask OS_DisableIrqMask(OSIrqMask mask);

/* Turn interrupts off and return the previous state; hand it back to restore. */
int OS_DisableInterrupts(void);
void OS_RestoreInterrupts(int state);

#endif
