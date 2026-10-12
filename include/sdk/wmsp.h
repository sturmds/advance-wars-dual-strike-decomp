#ifndef SDK_WMSP_H
#define SDK_WMSP_H

#include "types.h"
#include "sdk/os_arm7.h"
#include "sdk/mi.h"
#include "sdk/wm.h"

// WMSP is the ARM7 half of WM. It runs the requests the ARM9 sends and
// reports back through a reply buffer that both CPUs can see.

/* Wireless hardware configuration registers, named by address (GBATEK). */
#define IO_WIRELESS_INIT_REG_124 ((volatile u16 *)0x04808124)
#define IO_WIRELESS_INIT_REG_128 ((volatile u16 *)0x04808128)
#define IO_WIRELESS_INIT_REG_150 ((volatile u16 *)0x04808150)

#define WMSP_SLOT_COUNT 32
#define WMSP_SLOT_FLAG 0x8000
#define WMSP_WL_BUF_WORDS 128          /* a request handler's WL command buffer on the stack */

typedef struct WmspSlot {
    u32 flags;
    u32 unk_04;
    u32 unk_08;
    u8 pad_0c[4];
} WmspSlot;

/* An indication the WL driver sends up to WMSP in one of its message buffers
   (a WlBuffer in sdk/wl.h): code says what happened, the body follows. */
#define WIFI_HOSTMSG_PEER_AUTHED 0x84
#define WIFI_HOSTMSG_PEER_UNAUTHED 0x85
#define WIFI_HOSTMSG_PEER_JOINED 0x86
#define WIFI_HOSTMSG_PEER_REJOINED 0x87
#define WIFI_HOSTMSG_PEER_LEFT 0x88
#define WIFI_HOSTMSG_PARENT_GONE 0x8b
#define WIFI_HOSTMSG_BEACON_OUT 0x8c
#define WIFI_HOSTMSG_BEACON_HEARD 0x8d
#define WIFI_HOSTMSG_DATA_FRAME 0x180
#define WIFI_HOSTMSG_POLL_HEARD 0x182                /* a child received an MP frame */
#define WIFI_HOSTMSG_POLL_FINISHED 0x184            /* a parent received the children's MP replies */
#define WIFI_HOSTMSG_POLL_ACKED 0x185
#define WIFI_HOSTMSG_EVENT_BITS 0x186  /* best guess: WMSP disconnects every link on it */

/* The first 16 bytes of a WlIndication (sdk/wl.h), which the WMSP indication structs start with. */
typedef struct WlIndicationHeader {
    u8 bufferHeader[12];               /* the WlBuffer list links */
    u16 command;                       /* WIFI_HOSTMSG_* */
    u16 length;                        /* payload halfwords; WIFI_HOSTMSG_POLL_ACKED: 0 when WMSP made it up after a timeout */
} WlIndicationHeader;

typedef struct WlDeauthIndication {
    WlIndicationHeader ind;
    u8 macAddress[6];  /* the station that left */
    u16 reason;
} WlDeauthIndication;

typedef struct WlAssocIndication {
    WlIndicationHeader ind;
    u8 macAddress[6];  /* the station that joined */
    u16 aid;
    u8 pad_18[10];
    u8 ssid[24];
} WlAssocIndication;

/* A received data frame: a 0x2c-byte header, then length bytes of data. */
typedef struct WlDataFrame {
    u8 pad_00[6];
    u16 length;
    u8 pad_08[7];
    u8 rssi;           /* raw, see Wmsp_DecodeRssi */
    u8 pad_10[8];
    u8 destAdrs[6];
    u8 srcAdrs[6];
    u8 pad_24[8];
} WlDataFrame;

typedef struct WlDataIndication {
    WlIndicationHeader ind;
    WlDataFrame frame;
} WlDataIndication;

typedef struct WlMpIndication {
    WlIndicationHeader ind;
    RadioChildFrameBuffer frame;            /* frame.length + 0x30 bytes are valid */
} WlMpIndication;

typedef struct WlMpEndIndication {
    WlIndicationHeader ind;
    WMMpRecvBuf recv;
} WlMpEndIndication;

/* A parent's beacon, as a child receives it. */
typedef struct WlBeaconIndication {
    WlIndicationHeader ind;
    u8 pad_10[6];
    u16 gameInfoLength;
    u8 pad_18[0x24];
    WMGameInfo gameInfo;
} WlBeaconIndication;

/* The ack that ends an MP exchange. */
typedef struct WlMpAckFrame {
    u8 pad_00[0xc];
    u16 timeStamp;
    u16 rate_rssi;
    u8 pad_10[8];
    u8 destAdrs[6];
    u8 srcAdrs[6];
    u8 pad_24[6];
    u16 seqNum;
    u16 tmptt;
    u16 pollbmp;       /* bit n set: the child with aid n was polled */
} WlMpAckFrame;

typedef struct WlMpAckIndication {
    WlIndicationHeader ind;
    WlMpAckFrame frame;
} WlMpAckIndication;

/* The wireless hardware's free-running microsecond counter, low and high halves. */
#define IO_WIRELESS_USEC_LOW (*(vu16 *)0x048080f8)
#define IO_WIRELESS_USEC_HIGH (*(vu16 *)0x048080fa)

/* Requests WMSP queues for itself in a slot from Wmsp_TakeFlaggedSlot. */
typedef struct WmspAutoDeauthRequest {
    u32 apiid;                         /* WM_APIID_AUTO_DEAUTH */
    u8 macAddress[6];                  /* the station to deauthenticate */
} WmspAutoDeauthRequest;

typedef struct WmspAutoDisconnectRequest {
    u32 apiid;                         /* WM_APIID_AUTO_DISCONNECT */
    u32 aidBitmap;                     /* the stations to disconnect */
    u32 reason;
} WmspAutoDisconnectRequest;

typedef struct WmspInternalRequest {
    u32 apiid;
} WmspInternalRequest;

/* ARM7 side of WM: a command for the wireless (WL) firmware.
   words[] holds the arguments, then the reply header at words[index]:
   the argument count, then the command id. WL fills in the reply. */
typedef struct WlCommand {
    u16 reserved[6];                   /* the WlBuffer list links */
    u16 command;                       /* WL command id */
    u16 length;                        /* argument words that follow */
    u16 words[1];
} WlCommand;

/* A frame to send, the argument of WL command 0x100 (func_arm7_027c23b0):
   the 0x30-byte frame header of sdk/wl.h's WlTxFrame as WMSP fills it, with
   data pointing at the payload. Only the fields WMSP fills are named. */
typedef struct WmspTxFrame {
    u16 status;
    u8 pad_02[4];
    u16 length;                        /* bytes at data */
    u8 pad_08[6];
    u8 rate;                           /* in 100 kbit/s: 10 or 20 */
    u8 pad_0f[9];
    u8 destAdr[6];
    u8 srcAdr[6];
    u8 pad_24[8];
    const u16 *data;
} WmspTxFrame;

/* Stack space a WMSP handler gives a WL command and its reply. */
typedef union WlCommandBuf {
    WlCommand command;
    u8 bytes[0x200];
} WlCommandBuf;

/* The arguments of WL command 0x200, which sets the WL parameters in one go
   (written by WMSP_SetAllParams). The header is a WlCommand's. */
typedef struct WlParamSetAllCommand {
    u16 reserved[6];
    u16 length;
    u16 index;
    u8 macAddress[6];                  /* 0x10 */
    u16 unk_16;                        /* always 7 */
    u16 enableChannel;
    u8 pad_1a[2];
    u16 unk_1c;                        /* WMStatus.unk_e2 */
    u16 unk_1e;                        /* WMStatus.unk_1e8 */
    u16 wepMode;
    u16 wepKeyId;
    u8 wepKey[0x50];
    u16 unk_74;
    u16 unk_76;
    u16 unk_78;                        /* 0 when WMStatus.unk_e2 is 1, else 16 */
    u16 unk_7a;
    u8 unk_7c[8];                      /* cleared */
    u8 unk_84[24];                     /* filled with 0xff */
    u16 unk_9c;                        /* WMStatus.unk_1ea */
    u16 wepFlag;
} WlParamSetAllCommand;

/* The ARM9 asks the ARM7 to start WM with these pointers (the arguments of
   WM_Initialize's request). */
typedef struct WmspInitRequest {
    u16 apiid;
    u8 pad_02[2];
    void *arm7Buf;
    WMStatus *status;
    void *replyBuf;
} WmspInitRequest;

/* What the ARM7 writes into the reply buffer (the start of WMCallback). */
typedef struct WmspReply {
    u16 apiid;
    u16 errcode;
    u16 wlCmdID;
    u16 wlResult;
} WmspReply;

/* The reply to GET_W_COUNTER: the WL's counters, copied as they come. */
typedef struct WmspWCounterReply {
    WmspReply header;
    u8 counters[0xb4];
} WmspWCounterReply;

/* The ARM7 side's work area, reached through the pointer at its end. */
typedef struct WmspBuf {
    WMStatus *status;
    u8 pad_04[4];
    WmspReply *replyBuf;
    u8 pad_0c[0xc4];
    OSVAlarm vAlarm;
    WmspSlot slots[WMSP_SLOT_COUNT];
} WmspBuf;

/* What the ARM9 sends for a request with one value. */
typedef struct WmspRequest {
    u16 apiid;
    u8 pad_02[2];
    u32 arg;
} WmspRequest;

/* The arguments of WM_SetMPDataToPortEx's request. */
typedef struct WmspSetMPDataRequest {
    u16 apiid;
    u8 pad_02[2];
    const u16 *data;
    u32 size;
    u32 destBitmap;
    u32 port;
    u32 prio;
    void (*callback)(void *arg);
    void *arg;
} WmspSetMPDataRequest;

/* The arguments of WM_StartDCF's request. */
typedef struct WmspStartDCFRequest {
    u16 apiid;
    u8 pad_02[2];
    void *recvBuf;
    u32 recvBufSize;                   /* in bytes; split in two halves */
} WmspStartDCFRequest;

/* The arguments of WM_SetDCFData's request. */
typedef struct WmspSetDCFDataRequest {
    u16 apiid;
    u8 pad_02[2];
    u8 destAdr[6];                     /* sent as two words */
    u8 pad_0a[2];
    const u16 *data;
    u32 size;
} WmspSetDCFDataRequest;

/* The arguments of WM_SetWEPKey's and WM_SetWEPKeyEx's requests; only the
   second has keyId. */
typedef struct WmspSetWepKeyRequest {
    u16 apiid;
    u8 pad_02[2];
    u32 wepMode;
    const u8 *wepKey;
    u32 keyId;
} WmspSetWepKeyRequest;

/* The arguments of WM_SetGameInfo's request. */
typedef struct WmspSetGameInfoRequest {
    u16 apiid;
    u8 pad_02[2];
    u16 *userGameInfo;
    u32 userGameInfoLength;
    u32 ggid;
    u32 tgid;
    u32 attribute;                     /* WIRELESS_BEACON_BIT_* in the low byte */
} WmspSetGameInfoRequest;

/* The argument of WM_SetParentParameter's request. */
typedef struct WmspSetParentParamRequest {
    u16 apiid;
    u8 pad_02[2];
    const WMParentParam *param;
} WmspSetParentParamRequest;

/* The arguments of WM_SetLifeTime. The lifetimes count 100 ms steps. */
typedef struct WmspSetLifeTimeRequest {
    u16 apiid;
    u8 pad_02[2];
    u32 tgid;
    u32 camLifeTime;
    u32 frameLifeTime;
    u32 mpLifeTime;
} WmspSetLifeTimeRequest;

/* The request handlers, by apiid; the request thread runs one per request. */
#define WMSP_API_COUNT 43
extern void (*gWmspRequestHandlers[WMSP_API_COUNT])(void *request);

/* What the ARM9 sends for a request about one station. */
typedef struct WmspMacRequest {
    u16 apiid;
    u8 pad_02[2];
    u8 macAddress[6];
} WmspMacRequest;

// The symbol data_arm7_027d7c44, which is the same memory as WmspWork from
// 0x1000 on. Only the part WMSP uses.
typedef struct WmspState {
    u8 pad_000[0x50c];
    u32 unk_50c;
    u32 unk_510;
    u32 unk_514;
    WmspBuf *buf;
    u8 history[16];
    volatile u32 historyIndex;  /* volatile: needed to match, the original reloads it */
    u32 threadPriority[2][3];  /* [1] is what the WMSP threads were created with */
} WmspState;

// The work area starts 0x1000 bytes before data_arm7_027d7c44.
#define WMSP_STATE_OFFSET 0x1000

typedef struct WmspWork {
    OSMessageQueue wlQueue;            /* commands for the WL driver */
    OSMessage wlMessages[2];
    OSMessageQueue indicationQueue;    /* indications from the WL driver */
    OSMessage indicationMessages[4];
    OSMessageQueue wlReplyQueue;       /* the WL driver's answers to commands */
    OSMessage wlReplyMessages[4];
    OSMessageQueue requestQueue;       /* requests from the ARM9 (Wmsp_ReceiveRequest) and the ARM7 itself */
    OSMessage requestMessages[32];
    u8 pad_f8[0x1510 - 0xf8];
    s32 heapArena;                     /* where WMSP allocates from: Heap_TakeFirstFit_Arm7(heapArena, heap, size) */
    s32 heap;
    WmspBuf *buf;
} WmspWork;

#define WMSP_WORK ((WmspWork *)0x027d6c44)

extern WmspState data_arm7_027d7c44;

/* The parent parameters kept in WMStatus (see WMStatus.pparam). */
static inline WMParentParam *Wmsp_ParentParam(WMStatus *status)
{
    return (WMParentParam *)status->pparam;
}

/* The MP V-count alarm: one step per line, WMSP_VAlarmHandler dispatches on the line. */
#define WMSP_VALARM_DELAY 263               /* a whole frame of lines: the step is never skipped */

void WMSP_VAlarmHandler(void *line);

/* Sets WmspBuf.vAlarm for line, with the line as the handler's argument. */
static inline void Wmsp_SetVAlarm(WmspBuf *buf, s16 line)
{
    LineAlarm_ArmOneShot_Arm7(&buf->vAlarm, line, WMSP_VALARM_DELAY, WMSP_VAlarmHandler, (void *)line);
}

/* The wireless microsecond counter, low and next halfword (GBATEK W_US_COUNT0/1). */
#define W_US_COUNT0 (*(volatile u16 *)0x048080f8)
#define W_US_COUNT1 (*(volatile u16 *)0x048080fa)

/* A halfword in the BIOS area at the top of ARM7 WRAM, next to OS7_IRQ_CHECK_FLAGS:
   the microsecond counter at the start of a frame, in 64 us units. */
#define WMSP_FRAME_START_TIME (*(vu16 *)0x0380fff0)

/* Held while a reply buffer is filled and sent. */
extern OSMutex data_arm7_027d813c;

/* Take the buffer for a reply to the ARM9, fill it, and send it. */
void *Wmsp_GetCallbackBuffer(void);
void Wmsp_SendReply(void *reply);

/* The same 16 bytes as WmspState.history; the original names them apart. */
extern u8 data_arm7_027d8160[16];

WmspWork *func_arm7_027c05e0(void);
/* Sends the WL parameters kept in WMStatus; on failure replies to apiid and returns FALSE. */
BOOL WMSP_SetAllParams(int apiid, WlCommand *cmd);
void WMSP_ResetStatus(void);
int WMSP_InitWl(u16 *wlCmdID, u16 *wlResult);
void func_arm7_027c6eb4(u16 arg);
void func_arm7_027c6f68(u16 arg);
void func_arm7_027c70f8(void);


/* Sends the command to the firmware and returns its answer. */
WlCommand *Wmsp_SendWlCommand(WlCommand *command);

#endif
