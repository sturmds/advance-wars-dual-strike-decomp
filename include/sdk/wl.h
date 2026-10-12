#ifndef SDK_WL_H
#define SDK_WL_H

#include "types.h"


// WL is the ARM7 wireless driver below WM. Its work area address is kept at
// 0x0380fff4. The request handlers get a request block and fill a reply block.

typedef struct WlRequest {
    u8 pad_00[0x10];
    u16 unk_10;
    u16 unk_12;
} WlRequest;

typedef struct WlReply {
    u16 unk_00;
    u16 status;
    u16 unk_04;
    u16 value;
    u16 words[16];
} WlReply;

typedef struct WlLimits {
    u8 pad_00[0xd8];
    struct OSMessageQueue *toHost;   /* buffers the driver hands to the host side */
    struct OSMessageQueue *fromHost; /* buffers the host side hands to the driver */
    u8 pad_e0[0x14];
    u16 unk_f4;
    u16 unk_f6;
} WlLimits;

// Option bits at WlState offset 0x0e, set one by one by the parameter setters.
typedef struct WlFlags {
    u16 bit0 : 1;
    u16 bit1 : 1;
    u16 bit2 : 1;
    u16 bit3 : 1;
    u16 bit4 : 1;
    u16 bit5 : 1;
    u16 unused : 10;
} WlFlags;

// Frame buffer the driver builds; the payload starts at 0x24.
typedef struct WlTxFrame {
    u8 pad_00[0x24];
    u8 payload[10];
    u16 unk_2e;
} WlTxFrame;

typedef struct WlState {
    u16 unk_00;
    u16 phase;
    u16 unk_04;
    u16 unk_06;
    u16 unk_08;
    u16 unk_0a;
    u8 pad_0c[2];
    WlFlags flags;
    u16 unk_10;
    u8 pad_12[2];
    u32 unk_14;
    u8 pad_18[8];
    u16 mode;
    u8 pad_22[2];
    u16 unk_24;
    u16 unk_26;
    u8 pad_28[2];
    u16 unk_2a;
    u8 pad_2c[0xa];
    u16 unk_36;
    u16 words[16];
    u8 pad_58[0x2e];
    u16 unk_86;
    u16 unk_88;
    u8 pad_8a[2];
    u16 unk_8c;
    u8 pad_8e[6];
    u16 unk_94;
    u16 unk_96;
    u16 unk_98;
    u8 pad_9a[8];
    u16 unk_a2;
    u8 pad_a4[10];
    u16 unk_ae;
    u8 pad_b0[4];
    void *unk_b4;
    u8 pad_b8[0xc0];
    u16 unk_178;
    u8 pad_17a[6];
    WlTxFrame *txFrame;
} WlState;

/* Message buffers: a 12-byte header followed by the payload. A buffer sits
   in at most one list; listId names that list, 0 when it sits in none. */
#define WIFI_MSG_BUFFER_TAG 0xbf1d
#define WIFI_LIST_END ((WlBuffer *)-1)

typedef struct WlBuffer {
    struct WlBuffer *prev;
    struct WlBuffer *next;
    u16 listId;
    u16 magic;
} WlBuffer;

typedef struct WlBufferList {
    WlBuffer *first;
    WlBuffer *last;
    u16 count;
    u16 id;
} WlBufferList;

/* Where buffers come from. kind 0: Heap_TakeFirstFit_Arm7(arena, heap, size).
   kind 1: the alloc and free callbacks. */
typedef struct WlBufferAllocator {
    s32 kind;
    union {
        s32 arena;
        void *(*alloc)(s32 size);
    } a;
    union {
        s32 heap;
        void (*free)(void *ptr);
    } b;
} WlBufferAllocator;

/* Tasks run by the driver thread, one at a time. They wait in four FIFO
   queues by priority (0 first); next links the entries of a queue. */
#define WIFI_TASK_LIST_END 0xffff
#define WIFI_TASK_LEVELS 4

typedef struct WlTask {
    u16 next;
    u16 queued;
    void (*handler)(void);
} WlTask;

typedef struct WlWork {
    u16 queueHead[WIFI_TASK_LEVELS];
    u16 queueTail[WIFI_TASK_LEVELS];
    u16 topPriority;   /* lowest priority number that may hold a task */
    u16 runPriority;
    u16 runTask;
    u8 pad_016[0x7a];
    WlTask tasks[24];
    WlBufferAllocator allocator;
    u8 pad_15c[0x6c];
    WlBufferList outbox; /* buffers waiting to go to the host */
    WlBufferList inbox;  /* buffers received from the host */
    u8 pad_1e0[0x20];
    WlLimits limits;
    u8 pad_2f8[8];
    WlState state;
} WlWork;

#define WIFI_ANTENNA_SELECT (*(volatile u16 *)0x04808290)

#define WIFI_DRIVER_STATE (*(WlWork **)0x0380fff4)

void WifiTask_ThreadMain(void);
void WifiHost_TakeInboxBuffer(void **message);
void WifiTask_WaitForHost(void);
void WifiTask_Schedule(int priority, int task);
u16 WifiTask_PopNext(int priority);
int WifiBufList_PushTail(WlBufferList *list, WlBuffer *buffer);
int WifiBufList_PushTailIfLoose(WlBufferList *list, WlBuffer *buffer);
int WifiBufList_Unlink(WlBufferList *list, WlBuffer *buffer);
WlBuffer *WifiBuffer_New(WlBufferList *list, int size);
int WifiBuffer_Release(WlBufferList *list, WlBuffer *buffer);
int WifiBuffer_Transfer(WlBufferList *from, WlBufferList *to, WlBuffer *buffer);
void WifiBuffer_GiveToHost(WlBufferList *list, WlBuffer *buffer);

#endif
