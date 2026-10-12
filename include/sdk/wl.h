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

/* WlStation.state value checked before WL marks or clears a station's bits; meaning not known yet. */
#define WIFI_PEER_ASSOCIATED 0x40

/* One entry of the station table (28 bytes), reached by index. */
typedef struct WlStation {
    u16 state;
    u16 aid;
    u16 macAddress[3];
    u8 pad_0a[0x6];
    u16 unk_10; /* bit 1: send to this station at 2 Mbit/s */
    u8 pad_12[0x4];
    u16 queuedFrames;
    u16 unk_18; /* reloaded from unk_1a */
    u16 unk_1a;
} WlStation;

/* The wifi block of the firmware flash header, from offset 0x2a: a CRC-16
 * of the bytes from length on, then the length, then the data. */
#define WIFI_RADIO_BLOCK_FLASH_OFFSET 0x2a
#define WIFI_RADIO_BLOCK_SIZE_LOW 0xa4
#define WIFI_RADIO_BLOCK_SIZE_HIGH 0x1d6

typedef struct WlWifiConfig {
    u16 crc;
    u16 length;
} WlWifiConfig;

/* Results of WifiFlash_ValidateRadioBlock. */
enum {
    WIFI_RADIO_BLOCK_VALID = 0,
    WIFI_RADIO_BLOCK_CRC_WRONG = 1,
    WIFI_RADIO_BLOCK_SIZE_WRONG = 2
};

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

// Request of WifiCmd_ApplyAllSettings: one value for each parameter setter, in the order
// of WlStatusReply.
typedef struct WlParamsRequest {
    u8 pad_00[0x10];
    u16 macAddress[3];
    u16 work2fe;
    u16 mask00;
    u16 unk_1a;
    u16 phase;
    u16 value04;
    u16 value08;
    u16 value0a;
    u8 wifiRamBlocks[0x50];
    u16 optionBit0;
    u16 optionBit1;
    u16 value96;
    u16 value10;
    u16 words58[16];
    u16 optionBit2;
    u16 value06;
} WlParamsRequest;

// Reply of WifiCmd_ReportFirmwareInfo.
typedef struct WlVersionReply {
    u16 unk_00;
    u16 status;
    u16 unk_04;
    char version[8]; /* "2.39.00" */
    u16 chipId;      /* WIFI_MAC_REGISTER(0x000) */
    u16 bbChipId;    /* baseband register 0 */
    u16 bbCrc;       /* WifiBaseband_ChecksumStartValues */
    u16 rfType;
} WlVersionReply;

// Request of WifiCmd_RunRadioTest.
typedef struct WlTestModeRequest {
    u8 pad_00[0x10];
    u16 start;   /* 1 starts a test transmission, 0 stops it */
    u16 signal;  /* 0-2: WIFI_MODE_CARRIER_TEST, 3-4: WIFI_MODE_FRAME_TEST */
    u16 rate;    /* 10 or 20 */
    u16 channel;
} WlTestModeRequest;

// WlLink.mode values. Modes 0x10-0x1f are the ones where WifiCmd_RunRadioTest may run.
#define WIFI_MODE_RADIO_ON 16
#define WIFI_MODE_CARRIER_TEST 17
#define WIFI_MODE_FRAME_TEST 18 /* sends the frame in WIFI_PACKET_RAM again on every TX done */

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
    u32 unk_e0;
    u32 unk_e4;                      /* 0xffffffff when set up as 0 */
    u32 spiLockId;                 /* 0xe8 id WL passes to SPI_Lock */
    WlWifiConfig *wifiConfig;      /* 0xec copy of the flash wifi block, or NULL */
    u32 unk_f0;
    u16 unk_f4;
    u16 unk_f6;
} WlLimits;

/*
 * Per-station bitmasks (bit n for station n) and the association IDs handed
 * out (bit n of aidMask set while AID n is in use).
 */
typedef struct WlStationBits {
    u16 stationCount; /* station entries with a nonzero state */
    u16 unk_02;
    u16 unk_04;
    u16 unk_06;
    u16 aidCount;
    u16 aidMask;
} WlStationBits;

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
    u8 pad_0a[0xc];
    u16 testRate;   /* 10 or 20, from the WifiCmd_RunRadioTest request */
    u16 testTxActive;
    u8 pad_1a[0x62];
    u16 flags_7c;
    u8 pad_7e[0xc];
    u16 flags_8a;
    u8 pad_8c[0x20];
    u16 savedBbReg6; /* baseband register 6 before WifiTest_SendPatternFrames changed it */
} WlLink;

// Entry inside the frame payload at a running offset.
typedef struct WlPayloadEntry {
    u8 pad_00[6];
    u8 bytes[2];
} WlPayloadEntry;

// Frame buffer the driver builds; the payload starts at 0x24.
typedef struct WlTxFrame {
    u16 unk_00;
    u16 station;
    u8 pad_04[2];
    u16 unk_06;
    u16 result; /* set when the frame is done, returned in the reply */
    u8 pad_0a[0x12 - 0x0a];
    u16 unk_12;
    u8 pad_14[0x24 - 0x14];
    u8 payload[10];
    u16 unk_2e;
} WlTxFrame;

/* The host request being handled; task 11 takes it from the inbox. */
typedef struct WlCurrentRequest {
    struct WlBuffer *buffer;
    u16 busy; /* bit 0 set while buffer is being handled */
} WlCurrentRequest;

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
    u16 unk_78;
    u8 pad_7a[0x7c - 0x7a];
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
    u8 pad_9a[0xac - 0x9a];
    /* Wi-Fi RAM offset of the beacon frame that holds the TIM element. */
    u16 timOffset;
    u16 unk_ae;
    u8 pad_b0[0xb2 - 0xb0];
    u16 ringSize;                  /* length of the MAC RAM ring that ends at WIFI_RX_RING_LIMIT */
    void *unk_b4;
    u8 pad_b8[0xc0 - 0xb8];
    u32 periodicCount;             /* periodic alarm firings */
    u8 pad_c4[4];
    u16 unk_c8;
    u8 pad_ca[0xd8 - 0xca];
    u16 unk_d8[16];
    WlCurrentRequest current;
    u8 pad_100[0x178 - 0x100];
    u16 unk_178;
    u8 pad_17a[6];
    WlTxFrame *txFrame;
} WlState;

/* Kept at WlWork+0x318, inside the range WlState covers (data is WlState.unk_b4); the code reaches
   it through its own pointer. */
typedef struct WlUnk318 {
    u8 pad_00[0x9c];
    u8 *data;
    u16 dataLength;
    u16 flags; /* bit 0: data starts one byte after the pointer */
} WlUnk318;

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

// Host buffer that carries a frame to send. The reply sits replyOffset
// halfwords after the start of frame.
typedef struct WlTxRequest {
    WlBuffer header;
    u16 command;
    u16 replyOffset;
    WlTxFrame frame;
} WlTxRequest;

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

/* The buffer fields of WlWork from allocator to inbox, as one struct. Some
   functions reach them through a pointer to allocator. */
typedef struct WlBufferPool {
    WlBufferAllocator allocator;
    WlBufferList newBuffers;
    u8 pad_18[0x60];
    WlBufferList outbox;
    WlBufferList inbox;
} WlBufferPool;

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
    u8 pad_016[2];
    OSThread thread;   /* runs WifiTask_ThreadMain */
    WlTask tasks[24];
    WlBufferAllocator allocator;
    WlBufferList unk_15c;          /* holds the wifi config buffer */
    WlBufferList txQueues[2]; /* TX queues 0 and 1; queue 2 is multicastQueue */
    /* Multicast and broadcast frames; TIM bit 0 is set while it is not empty. */
    WlBufferList multicastQueue;
    u8 pad_18c[0x1c8 - 0x18c];
    WlBufferList outbox; /* buffers waiting to go to the host */
    WlBufferList inbox;  /* buffers received from the host */
    u8 pad_1e0[0x20];
    WlLimits limits;
    u16 macAddress[3];
    u16 unk_2fe;
    union {
        WlState state;
        struct {
            u8 pad_300[0x18];
            WlUnk318 unk_318;
        };
    };
    u8 pad_484[0x504 - 0x484];
    WlStationBits stationBits;
    u32 counters[45]; /* totals of the hardware event counters, see WifiCmd_ReportStatistics */
    WlRandom random;
    u8 pad_5ca[2];
    WlRfConfig rf;
    OSAlarm periodicAlarm;
    OSAlarm oneShotAlarm;
    u32 unk_634;
    u16 unk_638;
    u16 unk_63a;
} WlWork;

#define WIFI_ANTENNA_SELECT (*(volatile u16 *)0x04808290)
#define WIFI_MAC_PREAMBLE_BITS (*(volatile u16 *)0x048080bc)
#define WIFI_POST_BEACON_TIMER (*(volatile u16 *)0x04808134)
#define WIFI_MAC_MODE_AND_WEP (*(volatile u16 *)0x04808006)
#define WIFI_MAC_RETRY_LIMIT (*(volatile u16 *)0x0480802c)
#define WIFI_OWN_MAC_REGISTERS ((u16 *)0x04808018)
#define WIFI_MAC_RUN (*(volatile u16 *)0x04808004)
#define WIFI_IRQ_PENDING_FLAGS (*(volatile u16 *)0x04808010)
#define WIFI_IRQ_ENABLE_MASK (*(volatile u16 *)0x04808012)
#define WIFI_MAC_FORCE_POWER (*(volatile u16 *)0x04808040)
#define WIFI_TX_LOC1_FRAME_ADDR (*(volatile u16 *)0x048080a0)
#define WIFI_TX_REQUEST_CLEAR (*(volatile u16 *)0x048080ac)
#define WIFI_TX_REQUEST_RAISE (*(volatile u16 *)0x048080ae)
#define WIFI_TX_HEADER_CONTROL (*(volatile u16 *)0x04808194)
#define WIFI_TEST_TX_CONTROL (*(volatile u16 *)0x048081a0)
#define WIFI_TEST_TX_PATTERN (*(volatile u16 *)0x048081a2)
#define WIFI_TEST_TX_RATE (*(volatile u16 *)0x048081a4)

// Wireless RAM, as halfwords.
#define WIFI_PACKET_RAM ((volatile u16 *)0x04804000)

#define WIFI_LINK_VIEW_OF(work) ((WlLink *)(work)->state.pad_18)

#define WIFI_DRIVER_STATE (*(WlWork **)0x0380fff4)

#define WIFI_SSID_LONGEST 32

/* A message from the driver to the host: command, payload length in
   halfwords, then the payload. */
typedef struct WlIndication {
    WlBuffer buffer;
    u16 command;
    u16 length;
    union {
        struct {
            u8 mac[6];
            u16 value;
            u16 ssidLength;
            u8 ssid[WIFI_SSID_LONGEST];
        } peer; /* commands 0x84-0x88; 0x8b has the MAC only; the SSID only in 0x86 and 0x87 */
        struct {
            u8 pad_10[6];
            u16 dataLength;
            u8 pad_18[6];
            u8 unk_1e;
            u8 unk_1f;
            u8 pad_20[0xe];
            u8 mac[6];
            u8 pad_34[8];
            u8 data[1];
        } ind8d;
    };
} WlIndication;

typedef struct Unk_027cb9e4 {
    u8 pad_00[0xe];
    u16 unk_0e;
    u8 pad_10[2];
    u16 unk_12;
    u8 pad_14[0xa];
    u8 mac[6];
} Unk_027cb9e4;

/* Wireless hardware registers (names from GBATEK). */
#define REG_W_BB_BUSY   (*(volatile u16 *)0x0480815e) /* bit 0: baseband serial transfer busy */
#define REG_W_RF_PINS   (*(volatile u16 *)0x0480819c)
#define W_RF_PINS_CARRIER_SENSE 0x0001
#define W_RF_PINS_RX_ON         0x0080
#define REG_W_RF_STATUS (*(volatile u16 *)0x04808214)

/* Wi-Fi transmit request registers: writing a bit clears or sets that request. */
#define REG_W_TXREQ_RESET (*(volatile u16 *)0x048080ac)
#define REG_W_TXREQ_SET (*(volatile u16 *)0x048080ae)
#define W_TXREQ_LOC3 0x8
/* Wi-Fi RF serial status: bit 0 set while a transfer to the RF chip runs. */
#define REG_W_RF_BUSY (*(volatile u16 *)0x04808180)

/* Interrupt mask bit of the wireless interrupt; WL masks it around queue changes. */
#define IRQ_MASK_WIRELESS 0x1000000

WlStation *WifiStation_Entry(u16 index);

/* Station index returned when no station fits. */
#define WIFI_NO_PEER 0xff

/*
 * WlLimits 0xf0-0xf7 seen as one struct: unk_f0 holds the station table
 * address as a u32, unk_f6 the number of entries (entry 0 included).
 */
typedef struct WlStationTable {
    WlStation *stations;
    u16 maxCount;
    u16 count;
} WlStationTable;

#define WIFI_PEER_TABLE_INFO ((WlStationTable *)&WIFI_DRIVER_STATE->limits.unk_f0)
#define WIFI_PEER_ARRAY (WIFI_PEER_TABLE_INFO->stations)

/* TX header rate codes, in 100 kbit/s. */
#define WIFI_RATE_CODE_1M 10
#define WIFI_RATE_CODE_2M 20

// Wireless baseband (BB) and RF chip serial ports.
#define IO_WIRELESS_BASEBAND_SELECT (*(volatile u16 *)0x04808158)   /* index | 0x5000 write, | 0x6000 read */
#define IO_WIRELESS_BASEBAND_OUT (*(volatile u16 *)0x0480815a)
#define IO_WIRELESS_BASEBAND_IN (*(volatile u16 *)0x0480815c)
#define IO_WIRELESS_RADIO_WORD_HI (*(volatile u16 *)0x0480817c)  /* high half of the RF word */
#define IO_WIRELESS_RADIO_WORD_LO (*(volatile u16 *)0x0480817e)  /* low half; writing DATA2 starts the transfer */

BOOL WifiRf_AwaitSerialIdle(void);
// Returns 0 once the RF port is idle, 1 on timeout.
int WifiRf_SendWord(u32 value);
// Returns 0, or 0xffffffff when WifiBaseband_AwaitReady times out.
u32 WifiBaseband_Put(u32 index, u32 value);
u16 WifiBaseband_Get(u32 index);

/* Wireless hardware registers, 0x04808000 + offset. */
#define WIFI_MAC_REGISTER(offset) (*(volatile u16 *)(0x04808000 + (offset)))

/* One entry of a register setup table: write value to WIFI_MAC_REGISTER(offset). */
typedef struct WlRegValue {
    u16 offset;
    u16 value;
} WlRegValue;

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
int WifiBaseband_AwaitReady(void);
void Wifi_SpinForUs(u32 usec, OSAlarmHandler handler);
void WifiRf_AwaitReceiverOn(void);
void WifiTask_DrainOutbox(void);
int WifiHost_TellBeaconHeard(Unk_027cb9e4 *info);
int WifiHost_TellBeaconSent(void);
int WifiHost_TellBeaconLost(const u8 *mac);
int WifiHost_TellDisassociated(const u8 *mac, u16 value);
int WifiHost_TellReassociated(const u8 *mac, u16 value, const u8 *ssidElement);
int WifiHost_TellAssociated(const u8 *mac, u16 value, const u8 *ssidElement);
int WifiHost_TellDeauthenticated(const u8 *mac, u16 value);
int WifiHost_TellAuthenticated(const u8 *mac, u16 value);
void WifiHost_ReturnCurrentRequest(void);
void WifiRf_AwaitQuietAir(void);

#endif
