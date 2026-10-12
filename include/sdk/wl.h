#ifndef SDK_WL_H
#define SDK_WL_H

#include "types.h"
#include "sdk/os_arm7.h"

// WL is the ARM7 wireless driver below WM. Its work area address is kept at
// 0x0380fff4. The request handlers get a request block and fill a reply block.

typedef struct WlRequest {
    u8 pad_00[0xe];
    u16 unk_0e;
    u16 unk_10;
    u16 unk_12;
    u16 unk_14;
} WlRequest;

typedef struct WlReply {
    u16 unk_00;
    u16 status;
    u16 unk_04;
    u16 value;
    u16 words[16];
} WlReply;

// Reply of WifiCmd_ReportAllSettings. Field names give the WL work offset each one
// is copied from.
typedef struct WlStatusReply {
    u16 unk_00;
    u16 status;
    u16 unk_04;
    u16 macAddress[3];
    u16 work2fe;
    u16 state00;
    u16 state92;
    u16 phase;
    u16 state04;
    u16 state08;
    u16 state0a;
    u16 flag0;
    u16 flag1;
    u16 state96;
    u16 state10;
    u16 state58[16];
    u16 flag2;
    u16 state06;
} WlStatusReply;

// Entries of the table at WlLimits.unk_f0, indexed from 1 (see func_arm7_027cdd18).
typedef struct Unk_027cdd18 {
    u8 pad_00[0x18];
    u16 unk_18;
    u16 unk_1a;
} Unk_027cdd18;

typedef struct WlLimits {
    u8 pad_00[0xd8];
    struct OSMessageQueue *toHost;   /* buffers the driver hands to the host side */
    struct OSMessageQueue *fromHost; /* buffers the host side hands to the driver */
    u8 pad_e0[0xf0 - 0xe0];
    u32 unk_f0;
    u16 unk_f4;
    u16 unk_f6;
} WlLimits;

// Option bits at WlState offset 0x0e, set one by one by the parameter setters.
typedef union WlFlags {
    u16 raw;
    struct {
        u16 bit0 : 1;
        u16 bit1 : 1;
        u16 bit2 : 1;
        u16 bit3 : 1;
        u16 bit4 : 1;
        u16 bit5 : 1;
        u16 bit6 : 1;
        u16 unused : 9;
    };
} WlFlags;

// Second view of the work area, starting at WlState offset 0x18 (work offset
// 0x318). Its mode is the same halfword as WlState.mode.
typedef struct WlLink {
    u8 pad_00[8];
    u16 mode;
    u8 pad_0a[0x72];
    u16 flags_7c;
    u8 pad_7e[0xc];
    u16 flags_8a;
} WlLink;

// Entry inside the frame payload at a running offset.
typedef struct WlPayloadEntry {
    u8 pad_00[6];
    u8 bytes[2];
} WlPayloadEntry;

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
    u16 unk_12;                    /* bit 0x40: no alarm could be set up */
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
    u16 words_58[16];
    u8 pad_78[0x7c - 0x78];
    u16 unk_7c[3];
    u8 pad_82[4];
    u16 unk_86;
    u16 unk_88;
    u8 pad_8a[2];
    u16 unk_8c;
    u8 pad_8e[4];
    u16 unk_92;
    u8 pad_94[2];
    u16 unk_96;
    u16 unk_98;
    u8 pad_9a[0x14];
    u16 unk_ae;
    u8 pad_b0[0xb2 - 0xb0];
    u16 ringSize;                  /* length of the MAC RAM ring that ends at WIFI_RX_RING_LIMIT */
    void *unk_b4;
    u8 pad_b8[0xc0 - 0xb8];
    u32 unk_c0;
    u8 pad_c4[4];
    u16 unk_c8;
    u8 pad_ca[0x178 - 0xca];
    u16 unk_178;
    u8 pad_17a[6];
    WlTxFrame *txFrame;
} WlState;

/* Linear congruential generator: value = value * mul + add (16 bits). */
typedef struct WlRandom {
    u16 mul;
    u16 add;
    u16 value;
} WlRandom;

/* RF chip settings read from the firmware's wireless block (bytes 0x40-0x43, 0xce). */
typedef struct WlRfConfig {
    u16 type;                      /* firmware 0x40 */
    u16 bitsPerEntry;              /* firmware 0x41: bits 0-6 count, bit 7 a flag */
    u16 entryCount;                /* firmware 0x42 */
    u16 unk_06;                    /* firmware 0x43 */
    u8 unk_08;
    u8 pad_09[3];
    u32 unk_0c;
} WlRfConfig;

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
    u16 macAddress[3];
    u16 unk_2fe;
    WlState state;
    u8 pad_484[0x5c4 - 0x484];
    WlRandom random;
    u8 pad_5ca[2];
    WlRfConfig rf;
    OSAlarm periodicAlarm;
    OSAlarm oneShotAlarm;
} WlWork;

#define WIFI_ANTENNA_SELECT (*(volatile u16 *)0x04808290)
#define WIFI_MAC_PREAMBLE_BITS (*(volatile u16 *)0x048080bc)
#define WIFI_POST_BEACON_TIMER (*(volatile u16 *)0x04808134)
#define WIFI_MAC_MODE_AND_WEP (*(volatile u16 *)0x04808006)
#define WIFI_MAC_RETRY_LIMIT (*(volatile u16 *)0x0480802c)
#define WIFI_OWN_MAC_REGISTERS ((u16 *)0x04808018)

#define WIFI_LINK_VIEW_OF(work) ((WlLink *)(work)->state.pad_18)

#define WIFI_DRIVER_STATE (*(WlWork **)0x0380fff4)

/* End of the MAC RAM ring buffer read by WifiRx_CopyFromRingWrapped. */
#define WIFI_RX_RING_LIMIT 0x04805f60

/*
 * 32-bit time to OS tick conversions. Ticks run at the
 * system clock / 64; 33514 is the system clock / 1000.
 */
#ifndef Tick_FromMilliseconds32
#define Tick_FromMilliseconds32(ms) ((u32)(33514 * (u32)(ms)) / 64)
#define Tick_FromMicroseconds32(us) (Tick_FromMilliseconds32(us) / 1000)
#endif

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
