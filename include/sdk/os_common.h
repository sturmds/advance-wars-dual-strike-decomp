#ifndef SDK_OS_COMMON_H
#define SDK_OS_COMMON_H

#include "types.h"

/*
 * OS types and constants with the same layout on both CPUs. The thread
 * types that hold a saved CPU context differ: sdk/os.h has the ARM9 ones,
 * sdk/os_arm7.h the ARM7 ones, and each includes this header. Library
 * headers that only need the types below include this one, so ARM7 code
 * can use them too.
 */

typedef volatile u16 vu16;
typedef volatile u32 vu32;
typedef volatile u64 vu64;

#define REG_IME ((volatile u16*)0x04000208)
#define REG_IE ((volatile u32*)0x04000210)
#define REG_IF ((volatile u32*)0x04000214)

/* EXMEMCNT: external memory bus control. */
#define REG_EXMEMCNT (*(vu16*)0x04000204)

/* Two words in shared main RAM: bit set = lock ID free (IDs 0x40 to 0x7f). */
#define LOCK_ID_AVAILABLE_BITS ((volatile u32*)0x027fffb0)

/* Two words in shared main RAM: bit set = ARM7 lock ID free (IDs 0x80 to 0xbf). */
#define OS7_LOCK_ID_FREE_MAP ((volatile u32*)0x027fffb8)

#define OS_LOCK_ID_ERROR (-3) /* OS_GetLockID found no free id */
#define LOCK_NOT_OWNER (-2)

typedef u32 OSIrqMask;
typedef u32 OSIntrMode;

/* A word that the ARM9 and ARM7 take turns locking with a swap. */
typedef struct OSLockWord {
  volatile u32 lockFlag; /* 0 when free */
  u16 ownerID;
  u16 extension;
} OSLockWord;

/* The lock word for the Slot-2 cartridge bus, in shared main RAM. */
#define SLOTLOCK_WORD ((OSLockWord*)0x027fffe8)

/*
 * Lock word the two CPUs use to start up in step: the ARM9 sets ownerID to
 * 0x7f once it has set up its locks, the ARM7 answers with 0xbf in extension.
 */
#define BOOT_HANDSHAKE_WORD ((volatile OSLockWord*)0x027ffff0)
#define BOOT_HANDSHAKE_ARM9_READY 0x7f
#define BOOT_HANDSHAKE_ARM7_READY 0xbf

typedef void (*OSIrqFunction)(void);

/* One entry of the handler table that runs timer and DMA interrupts. */
typedef struct IrqSlotEntry {
  void (*handler)(void*); /* cleared after it runs once */
  BOOL keepEnabled;       /* when 0, the interrupt is disabled after it fires */
  void* arg;
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
  void* arg;
  u8 pad_04[8];
} IrqSlotArgAlias;

/* IE and IF bit: the receive FIFO is not empty. */
#define IRQ_FIFO_RECV 0x40000

typedef struct OSThread OSThread;
typedef struct OSMutex OSMutex;

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
  OSMutex* head;
  OSMutex* tail;
} OSMutexQueue;

struct OSMutex {
  OSThreadQueue queue;
  OSThread* thread;
  s32 count;
  OSMutex* prev;
  OSMutex* next;
};

typedef void* OSMessage;

enum { OS_MESSAGE_NOBLOCK = 0, OS_MESSAGE_BLOCK = 1 };

/*
 * queueSend and queueReceive are bit masks of waiting thread ids, the same
 * kind of word that OS_SleepThread and OS_WakeupThread take.
 */
typedef struct OSMessageQueue {
  u16 queueSend;
  u16 queueReceive;
  OSMessage* msgArray;
  s32 msgCount;
  s32 firstIndex;
  s32 usedCount;
} OSMessageQueue;

typedef void (*ThreadSwapHook)(OSThread* from, OSThread* to);

/* Scheduler state. The thread list is sorted by priority, lowest value first. */
typedef struct OSThreadInfo {
  u16 isNeedRescheduling;
  u16 maxEntry; /* number of entries in threads[] */
  u16 inIrq;
  u16 pad_6;
  OSThread* current;
  OSThread* list;
  ThreadSwapHook switchCallback;
  OSThread* threads[16];
} OSThreadInfo;

/* Memory areas the OS hands out as heaps. The ARM7 manages 1, 7 and 8. */
typedef enum OSArenaId {
  OS_ARENA_MAIN = 0,
  OS_ARENA_MAIN_SUBPRIV = 1, /* ARM7's private part of main RAM */
  OS_ARENA_MAINEX = 2,
  OS_ARENA_ITCM = 3,
  OS_ARENA_DTCM = 4,
  OS_ARENA_SHARED = 5,
  OS_ARENA_WRAM_MAIN = 6,
  OS_ARENA_WRAM_SUB = 7,    /* ARM7's share of the shared WRAM */
  OS_ARENA_WRAM_SUBPRIV = 8 /* ARM7's own WRAM */
} OSArenaId;

/* Arena bounds, indexed by OSArenaId, in shared main RAM. */
typedef struct OSArenaInfo {
  void* lo[9];
  void* hi[9];
} OSArenaInfo;

#define ARENA_BOUNDS_TABLE ((OSArenaInfo*)0x027ffda0)

/*
 * OS heaps carved out of an arena. Each block, used or free, starts with an
 * HeapBlockHeader header padded to HEAP_BLOCK_HEADER_BYTES; sizes include the header
 * and are multiples of 32.
 */
#define HEAP_BLOCK_HEADER_BYTES 32
#define HEAP_SMALLEST_SPLIT 64 /* a header and at least 32 bytes */

typedef struct HeapBlockHeader HeapBlockHeader;
struct HeapBlockHeader {
  HeapBlockHeader* prev;
  HeapBlockHeader* next;
  s32 size;
};

typedef struct HeapRecord {
  s32 size;              /* negative for an unused slot */
  HeapBlockHeader* free; /* sorted by address */
  HeapBlockHeader* allocated;
} HeapRecord;

/* One per arena that has heaps, at the start of the arena. */
typedef struct OSHeapInfo {
  s32 currentHeap;  /* used when a heap argument is negative */
  s32 numHeaps;     /* number of slots in heapArray */
  void* arenaStart; /* first byte after heapArray, 32-byte aligned */
  void* arenaEnd;
  HeapRecord* heapArray;
} OSHeapInfo;

/* Ticks of the OS tick counter: system clock / 64. */
typedef u64 OSTick;

typedef struct OSAlarm OSAlarm;
typedef void (*OSAlarmHandler)(void* arg);

/* A timer 1 alarm. A periodic one has period != 0 and fires at start + n * period. */
struct OSAlarm {
  OSAlarmHandler handler; /* NULL while the alarm is not set */
  void* arg;
  u32 tag;
  OSTick fire;
  OSAlarm* prev;
  OSAlarm* next;
  OSTick period;
  OSTick start;
};

/* Set alarms, soonest first. */
typedef struct AlarmList {
  OSAlarm* head;
  OSAlarm* tail;
} AlarmList;

/* An alarm that fires when the LCD reaches a given line (V count). */
typedef void (*OSVAlarmHandler)(void* arg);

typedef struct OSVAlarm OSVAlarm;
struct OSVAlarm {
  OSVAlarmHandler handler;
  void* arg;
  u32 tag; /* alarms with one tag can be cancelled together */
  u32 frame;
  s16 fire;  /* line to fire on */
  s16 delay; /* lines it may fire late */
  OSVAlarm* prev;
  OSVAlarm* next;
  BOOL period; /* TRUE: fires every frame */
  BOOL finish;
  BOOL canceled;
};

/* Set V-count alarms, soonest first. */
typedef struct OSVAlarmQueue {
  OSVAlarm* head;
  OSVAlarm* tail;
} OSVAlarmQueue;

/* V count lines per frame, blanking included. */
#define LINE_ALARM_FRAME_LINES 263
#define IRQ_BIT_LINE_MATCH 0x4
/* DISPSTAT: interrupt when VCOUNT reaches the line in bits 7 (line bit 8) and 8-15. */
#define DISPSTAT_VCOUNT_IRQ 0x20

/* Where now stands against a V-count alarm's frame and line. */
enum {
  LINE_ALARM_EARLY = 0,
  LINE_ALARM_ON_TIME = 1, /* reached, at most delay lines late */
  LINE_ALARM_TOO_LATE = 2 /* more than delay lines late */
};

/* Frame counter in shared main RAM, raised by the VBlank interrupt. */
#define SHARED_FRAME_COUNTER ((volatile u32*)0x027ffc3c)

/* Boot type word in shared main RAM; 2 means the program came over multiboot. */
#define BOOT_ORIGIN_WORD ((volatile u16*)0x027ffc40)
#define BOOT_ORIGIN_WIRELESS 2

/* Word handed to the program that starts after a reset. */
#define RESET_ARGUMENT_WORD ((volatile u32*)0x027ffc20)

/* The ARM7 is told to reset with this command over the PXI FIFO. */
#define OS_PXI_COMMAND_RESET 0x10

/* PXI FIFO tag of the OS: the ARM9 and ARM7 exchange reset commands under it. */
#define FIFO_TAG_RESET 12

/* The console owner's profile, as OS_GetOwnerInfo hands it out. */
typedef struct OSOwnerInfo {
  u8 favoriteColor;
  u8 birthMonth;
  u8 birthDay;
  u8 unk_03;
  u16 nickname[10]; /* UTF-16 */
  u16 nicknameLength;
  u16 comment[26]; /* UTF-16 */
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

#define FIRMWARE_SETTINGS_COPY ((FirmwareUserSettings*)0x027ffc80)

#endif
