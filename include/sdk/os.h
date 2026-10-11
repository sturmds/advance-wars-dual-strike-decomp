#ifndef SDK_OS_H
#define SDK_OS_H

#include "types.h"

#define REG_IME ((volatile u16 *)0x04000208)
#define REG_IE ((volatile u32 *)0x04000210)
#define REG_IF ((volatile u32 *)0x04000214)

/* Two words in shared main RAM: bit set = lock ID free (IDs 0x40 to 0x7f). */
#define LOCK_ID_AVAILABLE_BITS ((volatile u32 *)0x027fffb0)

typedef u32 OSIrqMask;
typedef u32 OSIntrMode;

/* A word that the ARM9 and ARM7 take turns locking with a swap. */
typedef struct OSLockWord {
    volatile u32 lockFlag;         /* 0 when free */
    u16 ownerID;
    u16 extension;
} OSLockWord;

typedef void (*OSIrqFunction)(void);

/* One entry of the handler table that runs timer and DMA interrupts. */
typedef struct IrqSlotEntry {
    void (*handler)(void *);       /* cleared after it runs once */
    BOOL keepEnabled;              /* when 0, the interrupt is disabled after it fires */
    void *arg;
} IrqSlotEntry;

/*
 * The original reads the keepEnabled and arg fields of each IrqSlotEntry
 * through their own symbols (data_022a98c0, data_022a98c4, and the same
 * addresses 0x30 bytes on for the timer slots), so these two views of the
 * table are needed to match.
 */
typedef struct {
    BOOL keepEnabled;
    u8 pad_04[8];
} IrqSlotKeepAlias;

typedef struct {
    void *arg;
    u8 pad_04[8];
} IrqSlotArgAlias;

extern IrqSlotEntry gOsIrqSlots[];
extern IrqSlotKeepAlias data_022a98c0[];
extern IrqSlotArgAlias data_022a98c4[];

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

#define OS_LOCK_ID_ERROR (-3) /* OS_GetLockID found no free id */

/* Disables interrupts and halts the CPU for good. */
void OS_Terminate(void);

/* Each of these returns the previous value of the register it changes. */
OSIrqMask OS_ResetRequestIrqMask(OSIrqMask mask);
OSIrqMask OS_DisableIrqMask(OSIrqMask mask);
OSIrqMask OS_EnableIrqMask(OSIrqMask mask);
OSIrqMask OS_SetIrqMask(OSIrqMask mask);

/* Sets the handler for every interrupt bit set in mask. */
void OS_SetIrqFunction(OSIrqMask mask, OSIrqFunction function);

/* Turn interrupts off and return the previous state; hand it back to restore. */
int OS_DisableInterrupts(void);
void OS_RestoreInterrupts(int state);
OSIntrMode OS_DisableInterrupts_IrqAndFiq(void);
OSIntrMode OS_RestoreInterrupts_IrqAndFiq(OSIntrMode state);

/* Stores setData in *destp and returns what was there (swp). */
u32 MI_SwapWord(u32 setData, volatile u32 *destp);

/*
 * Take and give back a lock word. ctrl, when given, runs while the lock
 * changes hands (for the cartridge it flips the bus owner). Lock returns the
 * old lock flag (0 means it was free and is now ours); unlock returns
 * LOCK_NOT_OWNER if lockID is not the owner, else 0.
 */
u32 OSi_DoLockByWord(u16 lockID, OSLockWord *lockp, void (*ctrl)(void), BOOL disableFiq);
int OSi_DoUnlockByWord(u16 lockID, OSLockWord *lockp, void (*ctrl)(void), BOOL disableFiq);
#define LOCK_NOT_OWNER (-2)

#endif
