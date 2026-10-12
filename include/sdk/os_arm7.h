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
    OS_THREAD_STATE_READY = 1
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

/*
 * The compiler's signed divide: the quotient comes back in r0 and the
 * remainder in r1, so a 64-bit return value holds both. The original calls
 * it by this name, so a plain % would not match.
 */
u64 func_arm7_03806d4c(s32 dividend, s32 divisor);
#define ARM7_REMAINDER(dividend, divisor) ((s32)(func_arm7_03806d4c((dividend), (divisor)) >> 32))

#endif
