#ifndef SDK_OS_H
#define SDK_OS_H

#include "types.h"
#include "sdk/os_common.h"

/*
 * ARM9 build of the OS: its thread types (the saved context has the
 * divider and square root unit state) and its functions. ARM7 code includes
 * sdk/os_arm7.h instead; both include sdk/os_common.h.
 */
#ifdef SDK_OS_ARM7_H
#error "sdk/os.h and sdk/os_arm7.h describe different OSContext layouts"
#endif

extern IrqSlotEntry gOsIrqSlots[];
extern IrqSlotKeepAlias data_022a98c0[];
extern IrqSlotArgAlias data_022a98c4[];

/* Saved CPU registers of a thread. */
typedef struct OSContext {
  u32 cpsr;
  u32 r[13];
  u32 sp;
  u32 lr;
  u32 pcPlus4;
  u32 spSvc;
  u8 cpContext[0x1c]; /* 0x48: divider and square root unit state */
} OSContext;

struct OSThread {
  OSContext context;
  u32 state;
  OSThread* next;
  u32 id;
  u32 priority;
  void* profiler;
  OSMutex* mutex; /* the mutex this thread is waiting for */
  OSMutexQueue mutexQueue;
  u32 stackTop;    /* lowest address of the stack */
  u32 stackBottom; /* one past its highest address */
  u32 stackWarningOffset;
  OSThreadQueue joinQueue;
};

void OS_InitContext(OSContext* context, u32 pc, u32 sp);

/* Bytes in one line of the ARM9 data cache. */
#define CACHE_LINE_SIZE 32

/* Data cache maintenance over a byte range. */
void DC_FlushRange(const void* start, u32 size);
void DC_InvalidateRange(void* start, u32 size);
void DC_StoreRange(const void* start, u32 size);

u32 OS_GetThreadPriority(const OSThread* thread);
BOOL OS_SetThreadPriority(OSThread* thread, u32 priority);

/* Disables interrupts and halts the CPU for good. */
void OS_Terminate(void);

/* Each of these returns the previous value of the register it changes. */
OSIrqMask OS_ResetRequestIrqMask(OSIrqMask mask);
OSIrqMask OS_DisableIrqMask(OSIrqMask mask);
OSIrqMask OS_EnableIrqMask(OSIrqMask mask);
OSIrqMask OS_SetIrqMask(OSIrqMask mask);

/* Sets the handler for every interrupt bit set in mask. */
/* IE bits OS_SetIrqFunction can give a handler: 0 to 21. */
#define IRQ_BIT_COUNT 22

void OS_SetIrqFunction(OSIrqMask mask, OSIrqFunction function);

/* Turn interrupts off and return the previous state; hand it back to restore. */
int OS_DisableInterrupts(void);
void OS_RestoreInterrupts(int state);
OSIntrMode OS_DisableInterrupts_IrqAndFiq(void);
OSIntrMode OS_RestoreInterrupts_IrqAndFiq(OSIntrMode state);

/* Stores setData in *destp and returns what was there (swp). */
u32 MI_SwapWord(u32 setData, volatile u32* destp);

/*
 * Take and give back a lock word. ctrl, when given, runs while the lock
 * changes hands (for the cartridge it hands over the bus). Try takes the
 * lock once and returns the old lock flag (0 means it was free and is now
 * ours); unlock returns LOCK_NOT_OWNER if lockID is not the owner, else 0.
 */
u32 OSi_DoTryLockByWord(u16 lockID, OSLockWord* lockp, void (*ctrl)(void),
                        BOOL disableFiq);
int OSi_DoUnlockByWord(u16 lockID, OSLockWord* lockp, void (*ctrl)(void),
                       BOOL disableFiq);

/* Hands out a free lock ID (0x40 to 0x7f), or OS_LOCK_ID_ERROR; gives one back. */
int OS_GetLockID(void);
void OS_ReleaseLockID(u16 lockID);

/* Busy-waits about cycles CPU cycles. */
void OS_SpinWait(u32 cycles);

/* Stores parameter for the next program and resets the console. Does not return. */
void Reset_StopAndRequestArm7(u32 parameter);

/* Sends a command word to the ARM7, retrying until the FIFO takes it. */
void OsTag_SendCommandWord(u32 command);

/* PXI receive callback: sets the reset flag when the ARM7 answers the reset command, else halts. */
void Reset_OnOsTagMessage(u32 tag, u32 data, int err);

/* Registers Reset_OnOsTagMessage once the ARM7 is ready for OS commands. */
void OS_InitReset(void);

/* Copies the console owner's profile out of the boot settings. */
void OS_GetOwnerInfo(OSOwnerInfo* info);

/* Nine VRAM banks (A to I) can each be locked by a lock ID. */
#define VRAM_LOCKABLE_BANKS 9
#define VRAM_ALL_BANK_BITS 0x1ff
extern u32 gOsVramLockedBanks; /* bit set = bank locked */
extern u16 gOsVramLockIds[VRAM_LOCKABLE_BANKS];

/* Releases the banks in the mask that lockID holds. */
void OSi_UnlockVram(u16 banks, u16 lockID);

/* Forgets every VRAM bank lock. */
void OSi_InitVramExclusive(void);

/* Scheduler state; OS_InitThread sets it up. */
extern OSThreadInfo OSi_ThreadInfo;
extern OSThread** data_022a9924; /* points at OSi_ThreadInfo.current */
extern OSThread data_022a9984;   /* the idle thread */

/* Main RAM past 4 MB may be used as an arena. */
extern BOOL data_022a9b78;

/* Timer 0 overflow count, and the flag OSi_CountUpTick clears. */
extern vu64 gOsiTickCounter;
extern u32 gOsiNeedResetTimer;

/* The record Exception_FillContextRecord writes when an exception is taken,
   and Exception_RunUserHook passes to the user hook. */
typedef struct ExceptionRecord {
  OSContext context; /* registers at the exception; sp and lr of its mode */
  u32 unk_64;
  u32 unk_68;
  u32 unk_6c; /* r0 on entry to Exception_FillContextRecord */
  u32 frame;  /* 0x70: the frame OSi_ExceptionHandler pushed */
  u32 cpsr;   /* 0x74: CPSR while the record is filled */
  u32 unk_78;
  u32 spsr; /* 0x7c: SPSR of the mode that took the exception */
} ExceptionRecord;

typedef char
    ExceptionRecord_size_check[sizeof(ExceptionRecord) == 0x80 ? 1 : -1];

/* Exception user hook (0 for none) and the context record passed to it. */
extern u32 data_022a9b7c;
extern ExceptionRecord data_022a9b88;

#endif
