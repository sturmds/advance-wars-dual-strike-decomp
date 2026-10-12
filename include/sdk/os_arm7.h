#ifndef SDK_OS_ARM7_H
#define SDK_OS_ARM7_H

#include "types.h"
#include "sdk/os_common.h"

/*
 * ARM7 build of the OS: its thread types (the saved context has no
 * coprocessor state, so OSThread is 0x78 bytes) and its registers and
 * symbols. ARM9 code includes sdk/os.h instead; both include
 * sdk/os_common.h.
 */
#ifdef SDK_OS_H
#error "sdk/os_arm7.h and sdk/os.h describe different OSContext layouts"
#endif

#define REG_TM0CNT_L (*(vu16 *)0x04000100)
#define REG_TM0CNT_H (*(vu16 *)0x04000102)
#define IRQ_MASK_TIMER0 0x8
/* Timer control: start, interrupt on overflow, count at the system clock / 64. */
#define TMCNT_START_IRQ_DIV64 0xc1
#define REG_TM1CNT_L (*(vu16 *)0x04000104)
#define REG_TM1CNT_H (*(vu16 *)0x04000106)
#define IRQ_MASK_TIMER1 0x10
#define IRQ_MASK_TIMER3 0x40
#define IRQ_SIO 0x80 /* serial port interrupt */

/*
 * ARM7 WRAM flag word, 8 bytes below the end of WRAM: the interrupt
 * handlers set the bits the SDK's IRQ wait routines poll (ARM7 copy of the
 * DTCM flags on ARM9).
 */
#define OS7_IRQ_CHECK_FLAGS (*(vu32 *)0x0380fff8)

OSTick Tick_Read64_Arm7(void);

/* Saved CPU registers of a thread. */
typedef struct OSContext {
    u32 cpsr;
    u32 r[13];
    u32 sp;
    u32 lr;
    u32 pcPlus4;
    u32 spSvc;
} OSContext;

struct OSThread {
    OSContext context;
    u32 state;
    OSThread *next;
    u32 id;
    u32 priority;
    void *profiler;
    OSMutex *mutex;                /* the mutex this thread is waiting for */
    OSMutexQueue mutexQueue;
    u32 stackTop;                  /* lowest address of the stack */
    u32 stackBottom;               /* one past its highest address */
    u32 stackWarningOffset;
    OSThreadQueue joinQueue;
};

/* Interrupts. The mask functions return the previous value of the register they change. */
void Irq_ResetWakeMaskAndFrames_Arm7(void);
void Irq_DispatchLowestPending_Arm7(void);
void Irq_WakeAndSwitchOnExit_Arm7(void);
void Irq_IgnoreUnused_Arm7(void);
void IrqSlot_CallAndClear_Arm7(int slot);
void VBlankIrq_CountAndCallSlot_Arm7(void);
void Dma0Irq_CallSlotHandler_Arm7(void);
void Dma1Irq_CallSlotHandler_Arm7(void);
void Dma2Irq_CallSlotHandler_Arm7(void);
void Dma3Irq_CallSlotHandler_Arm7(void);
void Timer0Irq_CallSlotHandler_Arm7(void);
void Timer1Irq_CallSlotHandler_Arm7(void);
void Timer2Irq_CallSlotHandler_Arm7(void);
void Timer3Irq_CallSlotHandler_Arm7(void);
OSIrqMask Irq_AckPending_Arm7(OSIrqMask mask);
OSIrqMask Irq_ClearEnableBits_Arm7(OSIrqMask mask);
OSIrqMask Irq_SetEnableBits_Arm7(OSIrqMask mask);
OSIrqMask Irq_ReplaceEnableBits_Arm7(OSIrqMask mask);
void Irq_InstallHandlers_Arm7(OSIrqMask mask, OSIrqFunction function);
void Timer_InstallIrqSlot_Arm7(int timer, void (*handler)(void *), void *arg);

/* CPSR interrupt bits: each returns the previous state; hand it back to restore. */
int Cpu_UnmaskIrq_Arm7(void);
int Cpu_MaskIrq_Arm7(void);
void Cpu_RestoreIrqBit_Arm7(int state);
OSIntrMode Cpu_MaskIrqFiq_Arm7(void);
OSIntrMode Cpu_RestoreIrqFiqBits_Arm7(OSIntrMode state);
u32 Cpu_CurrentMode_Arm7(void);

void System_StartServices_Arm7(void);
void Cpu_IdleUntilIrq_Arm7(void);
void Cpu_HaltForever_Arm7(void);

/* Reset: the ARM9 asks for it over PXI, then the ARM7 stops its hardware and jumps to the reset code. */
void Reset_ListenForRequest_Arm7(void);
void Reset_OnOsTagMessage_Arm7(u32 tag, u32 data, int err);
u16 Reset_WasRequested_Arm7(void);
void Reset_QuiesceAndRestart_Arm7(void);
void Cpu_BusyWaitCycles_Arm7(s32 cycles);

/*
 * Lock words. Try takes the lock once and returns the old lock flag (0: it
 * is ours now); Lock retries until it gets it. Unlock returns
 * LOCK_NOT_OWNER if lockID is not the owner, else 0. ctrl runs while the
 * lock changes hands.
 */
void LockId_HandshakeAndFreeIds_Arm7(void);
int LockId_Allocate_Arm7(void);
void LockId_Free_Arm7(u16 lockID);
u32 LockWord_SwapOnce_Arm7(u16 lockID, OSLockWord *lockp, void (*ctrl)(void), BOOL disableFiq);
s32 LockWord_SpinAcquire_Arm7(u16 lockID, OSLockWord *lockp, void (*ctrl)(void), BOOL disableFiq);
int LockWord_ClearIfOwner_Arm7(u16 lockID, OSLockWord *lockp, void (*ctrl)(void), BOOL disableFiq);
void LockWord_PauseBetweenTries_Arm7(void);
u16 LockWord_OwnerId_Arm7(OSLockWord *lock);
s32 SlotLock_TryOnce_Arm7(u16 lockID);
s32 SlotLock_Acquire_Arm7(u16 lockID);
s32 SlotLock_Release_Arm7(u16 lockID);
void SlotLock_ReleaseAlias_Arm7(u16 lockID);
void SlotLock_OnAcquireNoop_Arm7(void);
void SlotLock_OnReleaseNoop_Arm7(void);

/* Threads. Thread queues are bit masks of thread ids. */
void Thread_SetUpTableOnce_Arm7(void);
void Thread_Spawn_Arm7(OSThread *thread, void (*func)(void *), void *arg, void *stack, u32 stackSize, u32 priority);
void Thread_EndCurrent_Arm7(void);
void Thread_WaitOnQueue_Arm7(OSThreadQueue *queue);
void Thread_ReadyAllWaiters_Arm7(OSThreadQueue *queue);
void Thread_MakeReady_Arm7(OSThread *thread);
BOOL Thread_Reprioritize_Arm7(OSThread *thread, u32 priority);
void Thread_SwitchToFirstReady_Arm7(void);
u32 Thread_HoldSwitching_Arm7(void);
u32 Thread_ReleaseSwitching_Arm7(void);
void Thread_NapMilliseconds_Arm7(u32 msec);
void Thread_WakeAfterNap_Arm7(void *thread);
OSThread *Thread_FirstRunnable_Arm7(void);
void Thread_LinkByPriority_Arm7(OSThread *thread);
void Thread_Unlink_Arm7(OSThread *thread);
int Thread_FirstFreeId_Arm7(void);

/* Save returns 0 when it saves and 1 when Context_Resume_Arm7 resumes the context. */
int Context_Capture_Arm7(OSContext *context);
void Context_Resume_Arm7(OSContext *context);

/* Message queues; flags is OS_MESSAGE_BLOCK or OS_MESSAGE_NOBLOCK. */
void MsgQueue_SetUpRing_Arm7(OSMessageQueue *queue, OSMessage *msgArray, s32 msgCount);
BOOL MsgQueue_PushBack_Arm7(OSMessageQueue *queue, OSMessage msg, int flags);
BOOL MsgQueue_PopOldest_Arm7(OSMessageQueue *queue, OSMessage *msg, int flags);
BOOL MsgQueue_PeekOldest_Arm7(OSMessageQueue *queue, OSMessage *msg, int flags);

/* V-count alarms: the handler runs when VCOUNT reaches line, up to delay lines late. */
void LineAlarm_ArmOneShot_Arm7(OSVAlarm *alarm, s16 line, s16 delay, OSVAlarmHandler handler, void *arg);

/* Mutexes. */
void Mutex_Take_Arm7(OSMutex *mutex);
void Mutex_Release_Arm7(OSMutex *mutex);
void Mutex_AppendToOwner_Arm7(OSThread *thread, OSMutex *mutex);
void Mutex_UnlinkFromOwner_Arm7(OSThread *thread, OSMutex *mutex);
OSMutex *Mutex_PopOldestHeld_Arm7(OSThread *thread);
void Mutex_ReleaseAllHeld_Arm7(OSThread *thread);

/*
 * data_arm7_0380788c is the ARM7 copy of gOsIrqSlots: 0-3 DMA, 4-7 timers,
 * 8 VBlank. IrqSlot_CallAndClear loads the slot fields through their own literals,
 * so the keep and arg views have symbols of their own.
 */
extern IrqSlotEntry data_arm7_0380788c[];
extern IrqSlotKeepAlias data_arm7_03807890[];
extern IrqSlotArgAlias data_arm7_03807894[];
#define OS7_IRQ_SLOT_VBLANK 8

/* Bit per thread ID: threads to mark ready on the way out of an interrupt. */
extern u16 data_arm7_03807888;

/* The IRQ vector the BIOS jumps through: the address of the IRQ handler. */
typedef void (*IrqEntryPoint)(void);
#define BIOS_IRQ_HANDLER_PTR (*(IrqEntryPoint volatile *)0x0380fffc)

/* State of the nesting IRQ handler (IrqNest_DispatchByPriority), gOsNestedIrqWork. */
typedef struct OSiNestedIrqWork {
    u32 unk_00;
    void *stack;                   /* stack of the outermost handler; holds the thread's sp while it runs */
    u32 stackSize;
    IrqEntryPoint savedVector;       /* the handler it replaced */
} OSiNestedIrqWork;

extern OSiNestedIrqWork gOsNestedIrqWork;
void IrqNest_DispatchByPriority(void);

#define IRQ_NEST_STACK_BASE ((void *)0x0380fe80)
#define IRQ_NEST_STACK_BYTES 0x40

/*
 * The compiler's signed divide: the quotient comes back in r0 and the
 * remainder in r1, so a 64-bit return value holds both. The original calls
 * it by this name, so a plain % would not match.
 */
u64 func_arm7_03806d4c(s32 dividend, s32 divisor);
#define ARM7_REMAINDER(dividend, divisor) ((s32)(func_arm7_03806d4c((dividend), (divisor)) >> 32))

#endif
