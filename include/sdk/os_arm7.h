#ifndef SDK_OS_ARM7_H
#define SDK_OS_ARM7_H

#include "types.h"

/*
 * Thread, mutex and message queue types of the ARM7 build of the OS. They
 * have the names of sdk/os.h but a smaller OSContext (no coprocessor state),
 * so the two headers are never included together.
 */
#ifdef SDK_OS_H
#error "sdk/os_arm7.h and sdk/os.h describe different OSContext layouts"
#endif

typedef volatile u16 vu16;
typedef volatile u32 vu32;
typedef volatile u64 vu64;

#define REG_IF (*(vu32 *)0x04000214)
#define REG_TM0CNT_L (*(vu16 *)0x04000100)
#define REG_TM0CNT_H (*(vu16 *)0x04000102)
#define IRQ_MASK_TIMER0 0x8
/* Timer control: start, interrupt on overflow, count at the system clock / 64. */
#define TMCNT_START_IRQ_DIV64 0xc1
#define REG_TM1CNT_L (*(vu16 *)0x04000104)
#define REG_TM1CNT_H (*(vu16 *)0x04000106)
#define IRQ_MASK_TIMER1 0x10
#define IRQ_MASK_TIMER3 0x40
/* Interrupts the SDK waits for, set by the handlers (ARM7 copy of the DTCM flags on ARM9). */
#define WRAM_IRQ_SEEN_BITS (*(vu32 *)0x0380fff8)

/* Ticks of Tick_Read64_Arm7: system clock / 64. */
typedef u64 OSTick;

typedef struct OSAlarm OSAlarm;
typedef void (*OSAlarmHandler)(void *arg);

/* A timer 1 alarm. A periodic one has period != 0 and fires at start + n * period. */
struct OSAlarm {
    OSAlarmHandler handler;        /* NULL while the alarm is not set */
    void *arg;
    u32 tag;
    OSTick fire;
    OSAlarm *prev;
    OSAlarm *next;
    OSTick period;
    OSTick start;
};

/* Set alarms, soonest first. */
typedef struct AlarmList {
    OSAlarm *head;
    OSAlarm *tail;
} AlarmList;

OSTick Tick_Read64_Arm7(void);

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
} OSContext;

/* Threads waiting on something, as a bit mask of thread ids. */
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

#define IRQ_SIO 0x80 /* serial port interrupt */

typedef void *OSMessage;

enum {
    OS_MESSAGE_NOBLOCK = 0,
    OS_MESSAGE_BLOCK = 1
};

/* queueSend and queueReceive are bit masks of waiting thread ids. */
typedef struct OSMessageQueue {
    u16 queueSend;
    u16 queueReceive;
    OSMessage *msgArray;
    s32 msgCount;
    s32 firstIndex;
    s32 usedCount;
} OSMessageQueue;

typedef void (*ThreadSwapHook)(OSThread *from, OSThread *to);

/* Scheduler state at data_arm7_0380790c. The thread list is sorted by priority, lowest value first. */
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

/* Arenas the ARM7 manages, numbered as in sdk/os.h's OSArenaId. */
typedef enum ArenaRegionArm7 {
    OS_ARENA_MAIN_SUBPRIV = 1,     /* ARM7's private part of main RAM */
    OS_ARENA_WRAM_SUB = 7,         /* ARM7's share of the shared WRAM */
    OS_ARENA_WRAM_SUBPRIV = 8      /* ARM7's own WRAM */
} ArenaRegionArm7;

/*
 * OS heaps carved out of an arena. Each block, used or free, starts with an
 * HeapBlockHeader header padded to HEAP_BLOCK_HEADER_BYTES; sizes include the header
 * and are multiples of 32.
 */
#define HEAP_BLOCK_HEADER_BYTES 32
#define HEAP_SMALLEST_SPLIT 64          /* a header and at least 32 bytes */

typedef struct HeapBlockHeader HeapBlockHeader;
struct HeapBlockHeader {
    HeapBlockHeader *prev;
    HeapBlockHeader *next;
    s32 size;
};

typedef struct HeapRecord {
    s32 size;                      /* negative for an unused slot */
    HeapBlockHeader *free;              /* sorted by address */
    HeapBlockHeader *allocated;
} HeapRecord;

/* One per arena that has heaps, at the start of the arena. */
typedef struct OSHeapInfo {
    s32 currentHeap;               /* used when a heap argument is negative */
    s32 numHeaps;                  /* number of slots in heapArray */
    void *arenaStart;              /* first byte after heapArray, 32-byte aligned */
    void *arenaEnd;
    HeapRecord *heapArray;
} OSHeapInfo;

/*
 * The compiler's signed divide: the quotient comes back in r0 and the
 * remainder in r1, so a 64-bit return value holds both. The original calls
 * it by this name, so a plain % would not match.
 */
u64 func_arm7_03806d4c(s32 dividend, s32 divisor);
#define ARM7_REMAINDER(dividend, divisor) ((s32)(func_arm7_03806d4c((dividend), (divisor)) >> 32))

#endif
