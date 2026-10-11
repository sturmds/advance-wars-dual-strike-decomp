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
typedef volatile u64 vu64;

/* EXMEMCNT: external memory bus control. */
#define REG_EXMEMCNT (*(vu16 *)0x04000204)

typedef struct OSThread OSThread;
typedef struct OSMutex OSMutex;

/* Saved CPU registers of a thread. */
typedef struct OSContext {
    u32 cpsr;
    u32 r[13];
    u32 sp;
    u32 lr;
    u32 pcPlus4;
    u32 spSvc;
    u8 cpContext[0x1c];            /* 0x48: divider and square root unit state */
} OSContext;

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

/* Mutexes a thread holds, oldest first. */
typedef struct OSMutexQueue {
    OSMutex *head;
    OSMutex *tail;
} OSMutexQueue;

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

struct OSMutex {
    OSThreadQueue queue;
    OSThread *thread;
    s32 count;
    OSMutex *prev;
    OSMutex *next;
};

typedef void *OSMessage;

enum {
    OS_MESSAGE_NOBLOCK = 0,
    OS_MESSAGE_BLOCK = 1
};

/*
 * queueSend and queueReceive are bit masks of waiting thread ids, the same
 * kind of word that OS_SleepThread and OS_WakeupThread take.
 */
typedef struct OSMessageQueue {
    u16 queueSend;
    u16 queueReceive;
    OSMessage *msgArray;
    s32 msgCount;
    s32 firstIndex;
    s32 usedCount;
} OSMessageQueue;

typedef void (*ThreadSwapHook)(OSThread *from, OSThread *to);

/* Scheduler state. The thread list is sorted by priority, lowest value first. */
typedef struct OSThreadInfo {
    u16 isNeedRescheduling;
    u16 maxEntry;                  /* number of entries in threads[] */
    u16 inIrq;
    u16 pad_6;
    OSThread *current;
    OSThread *list;
    ThreadSwapHook switchCallback;
    OSThread *threads[16];
} OSThreadInfo;

/* Memory areas the OS hands out as heaps. */
typedef enum OSArenaId {
    OS_ARENA_MAIN = 0,
    OS_ARENA_MAIN_SUBPRIV = 1,
    OS_ARENA_MAINEX = 2,
    OS_ARENA_ITCM = 3,
    OS_ARENA_DTCM = 4,
    OS_ARENA_SHARED = 5,
    OS_ARENA_WRAM_MAIN = 6
} OSArenaId;

void OS_InitContext(OSContext *context, u32 pc, u32 sp);

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

/* Busy-waits about cycles CPU cycles. */
void OS_SpinWait(u32 cycles);

/* Boot type word in shared main RAM; 2 means the program came over multiboot. */
#define BOOT_ORIGIN_WORD ((volatile u16 *)0x027ffc40)
#define BOOT_ORIGIN_WIRELESS 2

/* Word handed to the program that starts after a reset. */
#define RESET_ARGUMENT_WORD ((volatile u32 *)0x027ffc20)

/* The ARM7 is told to reset with this command over the PXI FIFO. */
#define OS_PXI_COMMAND_RESET 0x10

/* IE and IF bit: the receive FIFO is not empty. */
#define IRQ_FIFO_RECV 0x40000

/* Stores parameter for the next program and resets the console. Does not return. */
void Reset_StopAndRequestArm7(u32 parameter);

/* PXI FIFO tag of the OS: the ARM9 and ARM7 exchange reset commands under it. */
#define FIFO_TAG_RESET 12

/* Sends a command word to the ARM7, retrying until the FIFO takes it. */
void OsTag_SendCommandWord(u32 command);

/* PXI receive callback: sets the reset flag when the ARM7 answers the reset command, else halts. */
void Reset_OnOsTagMessage(u32 tag, u32 data, int err);

/* Registers Reset_OnOsTagMessage once the ARM7 is ready for OS commands. */
void OS_InitReset(void);

/* The console owner's profile, as OS_GetOwnerInfo hands it out. */
typedef struct OSOwnerInfo {
    u8 favoriteColor;
    u8 birthMonth;
    u8 birthDay;
    u8 unk_03;
    u16 nickname[10];          /* UTF-16 */
    u16 nicknameLength;
    u16 comment[26];           /* UTF-16 */
    u16 commentLength;
} OSOwnerInfo;

/* The user settings the boot code leaves in shared main RAM (0x027ffc80). */
typedef struct FirmwareUserSettings {
    u8 unk_00[2];
    u8 birthMonth : 4;
    u8 : 4;
    u8 birthDay;
    u8 unk_04;
    u8 pad_05;
    u16 nickname[10];
    u8 nicknameLength;
    u8 pad_1b;
    u16 comment[26];
    u8 commentLength;
    u8 pad_51[0x7];
    /* Touch panel calibration from the system settings: the raw reading at
     * two screen points (see TP_GetUserInfo). */
    u16 tpRawX1;
    u16 tpRawY1;
    u8 tpDotX1;
    u8 tpDotY1;
    u16 tpRawX2;
    u16 tpRawY2;
    u8 tpDotX2;
    u8 tpDotY2;
    u16 favoriteColor : 3;
    u16 : 13;
} FirmwareUserSettings;

#define FIRMWARE_SETTINGS_COPY ((FirmwareUserSettings *)0x027ffc80)

/* Copies the console owner's profile out of the boot settings. */
void OS_GetOwnerInfo(OSOwnerInfo *info);

/* Nine VRAM banks (A to I) can each be locked by a lock ID. */
#define VRAM_LOCKABLE_BANKS 9
#define VRAM_ALL_BANK_BITS 0x1ff
extern u32 gOsVramLockedBanks;   /* bit set = bank locked */
extern u16 gOsVramLockIds[VRAM_LOCKABLE_BANKS];

/* Releases the banks in the mask that lockID holds. */
void OSi_UnlockVram(u16 banks, u16 lockID);

/* Forgets every VRAM bank lock. */
void OSi_InitVramExclusive(void);

#endif
