#ifndef SDK_WM_H
#define SDK_WM_H

#include "types.h"
#include "sdk/os_common.h"

/* WM flags in the system work area shared with the ARM7 (the field
 * OSSystemWork.wm_callback_control). */
#define WIRELESS_CALLBACK_LOCK_WORD ((u16 *)0x027fff96)
#define WIRELESS_CALLBACK_BUSY_BIT 0x0001

/* What the callbacks passed to WM_PowerOff and its siblings receive. The
 * layout is the common WM callback header (apiid, errcode, ...); the wireless purpose is a guess from
 * that. errcode is 0 on success. */
typedef struct WMCallback {
    u16 apiid;
    u16 errcode;
} WMCallback;

/* The callback structs below share that header. state is a WIRELESS_NOTICE_*
 * value. */
#define WM_STATECODE_PARENT_START 0
#define WM_STATECODE_BEACON_SENT 2
#define WM_STATECODE_SCAN_START 3
#define WM_STATECODE_PARENT_NOT_FOUND 4
#define WM_STATECODE_PARENT_FOUND 5
#define WM_STATECODE_CONNECT_START 6
#define WM_STATECODE_CONNECTED 7
#define WM_STATECODE_BEACON_LOST 8
#define WM_STATECODE_DISCONNECTED 9
#define WM_STATECODE_MP_START 10
#define WM_STATECODE_MPEND_IND 11
#define WM_STATECODE_MP_IND 12
#define WM_STATECODE_MPACK_IND 13
#define WIRELESS_NOTICE_DCF_FRAME 15
#define WIRELESS_NOTICE_HEARD_BEACON 16
#define WIRELESS_NOTICE_STATION_DROPPED 17
#define WIRELESS_NOTICE_STATION_BACK 18
#define WIRELESS_NOTICE_AUTHED 19
#define WIRELESS_NOTICE_DCF_BEGUN 14
#define WM_STATECODE_PORT_SEND 20
#define WM_STATECODE_PORT_RECV 21
#define WIRELESS_NOTICE_QUEUE_REFUSED 22

/* WMCallback.errcode values. */
#define WM_ERRCODE_SUCCESS 0
#define WM_ERRCODE_FAILED 1
#define WM_ERRCODE_OPERATING 2             /* the command was sent and its callback will follow */
#define WM_ERRCODE_ILLEGAL_STATE 3
#define WM_ERRCODE_WM_DISABLE 4            /* the ARM7 side of WM is not running */
#define WM_ERRCODE_NO_DATASET 5
#define WM_ERRCODE_INVALID_PARAM 6
#define WM_ERRCODE_NO_CHILD 7
#define WM_ERRCODE_FIFO_ERROR 8
#define WM_ERRCODE_TIMEOUT 9
#define WM_ERRCODE_SEND_QUEUE_FULL 10
#define WM_ERRCODE_NO_ENTRY 11
#define WM_ERRCODE_OVER_MAX_ENTRY 12
#define WM_ERRCODE_INVALID_POLLBITMAP 13
#define WM_ERRCODE_NO_DATA 14
#define WM_ERRCODE_SEND_FAILED 15
#define WM_ERRCODE_FLASH_ERROR 19          /* the ARM7 could not read the wireless firmware */

#define WM_ATTR_FLAG_ENTRY 0x01  /* the parent accepts new children */
#define WM_ATTR_FLAG_MB 0x02     /* the parent offers a download play game */
#define WIRELESS_BEACON_BIT_KEYSHARE 0x04     /* the parent uses key sharing */
#define WIRELESS_BEACON_BIT_STREAMING 0x08     /* the parent uses continuous sending */

/* WMCallback.apiid value of the calls made here. */
#define WM_APIID_SET_MP_DATA 15

/* Command ids that the WM commands send to the ARM7, one per request kind. */
#define WM_APIID_INITIALIZE 0
#define WM_APIID_RESET 1
#define WM_APIID_ENABLE 3
#define WM_APIID_DISABLE 4
#define WM_APIID_POWER_ON 5
#define WM_APIID_POWER_OFF 6
#define WM_APIID_SET_P_PARAM 7
#define WM_APIID_START_PARENT 8
#define WM_APIID_END_PARENT 9
#define WM_APIID_START_SCAN 10
#define WM_APIID_END_SCAN 11
#define WM_APIID_START_CONNECT 12
#define WM_APIID_DISCONNECT 13
#define WM_APIID_RESET 1
#define WM_APIID_END 2
#define WM_APIID_SET_LIFETIME 29
#define WM_APIID_START_MP 14
#define WM_APIID_SET_MP_DATA 15
#define WM_APIID_END_MP 16
#define WM_APIID_START_DCF 17
#define WM_APIID_SET_DCF_DATA 18
#define WM_APIID_END_DCF 19
#define WM_APIID_SET_WEPKEY 20
#define WM_APIID_START_TESTMODE 26
#define WM_APIID_SET_WEPKEY_EX 39
#define WM_APIID_START_KS 21                /* best guess from the neighbouring request ids */
#define WM_APIID_SET_GAMEINFO 24
#define WM_APIID_SET_BEACON_IND 25
#define WM_APIID_STOP_TESTMODE 27
#define WM_APIID_VALARM_MP 28               /* queued by the ARM7 itself from its V-count alarm */
#define WM_APIID_SET_LIFETIME 29
#define WM_APIID_MEASURE_CHANNEL 30
#define WM_APIID_INIT_W_COUNTER 31
#define WM_APIID_GET_W_COUNTER 32
#define WM_APIID_SET_ENTRY 33
#define WM_APIID_AUTO_DISCONNECT 37
#define WM_APIID_AUTO_DEAUTH 34
#define WM_APIID_SET_BEACON_PERIOD 36      /* best guess from the neighbouring request ids */
#define WM_APIID_START_SCAN_EX 38          /* request slot 38 runs the extended scan */
#define WM_NUM_OF_CALLBACK 41              /* ids below this have a slot in callbackTable */
#define WM_APIID_INDICATION 0x80
#define WM_APIID_PORT_SEND 0x81
#define WM_APIID_PORT_RECV 0x82

#define PXI_PROC_ARM7 1
#define PXI_FIFO_TAG_WM 10                 /* PXI FIFO tag of messages between the WM halves */
#define PXI_FIFO_TAG_WVR 15                /* best guess: the wireless library kept in ARM7 VRAM */
#define WM_NUM_OF_PORT 16
#define WM_FIFO_BUF_SIZE 0x100             /* one command or reply in shared memory */
/* The work area given to WM_Init holds, in order: WMArm9Buf, the ARM7 side's
 * work area, the WMStatus block, the request buffer and the reply buffer. */
#define WM_ARM9WM_BUF_SIZE 0xa00
#define WM_ARM7WM_BUF_SIZE 0x300
#define WM_STATUS_BUF_SIZE 0x800
#define WM_SYSTEM_BUF_SIZE (WM_ARM9WM_BUF_SIZE + WM_ARM7WM_BUF_SIZE + WM_STATUS_BUF_SIZE + 2 * WM_FIFO_BUF_SIZE)
#define WIRELESS_CMD_BUFFERS 10               /* command buffers the ARM9 can have in flight */
#define WM_API_REQUEST_ACCEPTED 0x8000     /* set in a buffer's apiid once it may be reused */

/* What an indication callback receives (unsolicited ARM7 events). */
typedef struct RadioNotice {
    u16 apiid;
    u16 errcode;
    u16 state;                         /* WIRELESS_NOTICE_* */
    u16 reason;                        /* WIRELESS_NOTICE_QUEUE_REFUSED: the apiid the ARM7 could not queue */
} RadioNotice;

/* Start of the reply to the DCF start command. */
typedef struct RadioDcfReply {
    u16 apiid;
    u16 errcode;
    u16 state;                         /* WIRELESS_NOTICE_DCF_BEGUN */
    u8 pad_06[2];
    void *recvBuf;                     /* DCF_IND: the buffer the frame was copied into */
} RadioDcfReply;

typedef struct WMGameInfo {
    u16 magicNumber;
    u8 ver;
    u8 platform;
    u32 ggid;                          /* game group id */
    u16 tgid;
    u8 userGameInfoLength;
    u8 attribute;                      /* WIRELESS_BEACON_BIT_* */
    u16 parentMaxSize;
    u16 childMaxSize;
    u8 userGameInfo[112];              /* layout chosen by the game */
} WMGameInfo;

/* A parent's beacon, as a scan reports it (0xc0 bytes). */
typedef struct WMBssDesc {
    u16 length;                        /* in halfwords */
    u16 rssi;
    u8 bssid[6];
    u16 ssidLength;
    u8 ssid[32];
    u16 capaInfo;
    u16 rateSetBasic;
    u16 rateSetSupport;
    u16 beaconPeriod;
    u16 dtimPeriod;
    u16 channel;
    u16 cfpPeriod;
    u16 cfpMaxDuration;
    u16 gameInfoLength;
    u16 otherElementCount;
    WMGameInfo gameInfo;
} WMBssDesc;

typedef struct WMStartScanCallback {
    u16 apiid;
    u16 errcode;
    u16 wlCmdID;
    u16 wlResult;
    u16 state;
    u8 macAddress[6];
    u16 channel;
    u16 linkLevel;
    u16 ssidLength;
    u8 ssid[32];
    u16 gameInfoLength;
    WMGameInfo gameInfo;
} WMStartScanCallback;

/* What the scan request reads. */
typedef struct WMScanParam {
    WMBssDesc *scanBuf;                /* where a found parent is written */
    u16 channel;
    u16 maxChannelTime;                /* milliseconds spent on the channel */
    u8 bssid[6];                       /* all 0xff scans for any parent */
} WMScanParam;

typedef struct WMMeasureChannelCallback {
    u16 apiid;
    u16 errcode;
    u16 wlCmdID;
    u16 wlResult;
    u16 channel;
    u16 ccaBusyRatio;                  /* percent of the time the channel was busy */
} WMMeasureChannelCallback;

typedef struct WMStartParentCallback {
    u16 apiid;
    u16 errcode;
    u16 wlCmdID;
    u16 wlResult;
    u16 state;
    u8 macAddress[6];
    u16 aid;                           /* the child's id, 1 to 15 */
    u16 reason;
    u8 ssid[24];
} WMStartParentCallback;

/* Start of the reply to the MP start command; only these fields are read. */
typedef struct WMStartMPCallback {
    u16 apiid;
    u16 errcode;
    u16 state;
    u8 pad_06[2];
    void *recvBuf;
    u16 timeStamp;                     /* MPACK_IND: copied from the ack frame */
    u16 rate_rssi;
    u8 destAdrs[6];
    u8 srcAdrs[6];
    u16 seqNum;
    u16 tmptt;
    u16 pollbmp;
} WMStartMPCallback;

/* Start of the reply to the disconnect command; only these fields are read. */
typedef struct RadioDropReply {
    u16 apiid;
    u16 errcode;
    u16 wlCmdID;
    u16 wlResult;
    u16 tryBitmap;
    u16 disconnectedBitmap;            /* bit n set: the child with aid n left */
} RadioDropReply;

/* Start of the reply to the connect command; only these fields are read. */
typedef struct WMStartConnectCallback {
    u16 apiid;
    u16 errcode;
    u16 wlCmdID;
    u16 wlResult;
    u16 state;
    u16 aid;                           /* the id the parent gave this child */
    u16 reason;
} WMStartConnectCallback;

typedef struct WMPortRecvCallback {
    u16 apiid;
    u16 errcode;
    u16 state;
    u16 port;
    void *recvBuf;
    void *data;
    u16 length;
    u16 aid;                           /* the sender */
    u8 macAddress[6];                  /* who connected or disconnected */
    u16 seqNo;                         /* 0xffff: none */
    void *arg;                         /* what WM_SetPortCallback was given */
    u16 myAid;
    u8 pad_22[2];
    u8 ssid[24];
} WMPortRecvCallback;

/* Start of the reply to a port send; only these fields are read. */
typedef struct WMPortSendCallback {
    u16 apiid;
    u16 errcode;
    u16 wlCmdID;
    u16 wlResult;
    u16 state;                         /* WM_STATECODE_PORT_SEND */
    u16 port;
    u16 destBitmap;
    u16 restBitmap;                    /* children the data has not reached yet */
    u16 sentBitmap;
    u8 pad_12[2];
    const u16 *data;
    u16 size;
    u16 seqNo;
    void (*callback)(void *arg);       /* run when the data has been sent */
    void *arg;                         /* what the sender passed with the data */
} WMPortSendCallback;

/* One child's data in an MP receive buffer; length 0xffff or 0 means none
 * arrived. Only these fields are read so far. */
typedef struct WMMpRecvData {
    u16 length;
    u16 unk_02;
    u16 aid;                           /* the sender */
    u16 noResponse;
    u16 timeStamp;
    u8 cdata[2];                       /* the data, length bytes */
} WMMpRecvData;

/* Receive buffer of an MP exchange: count entries, stride bytes apart. */
typedef struct WMMpRecvBuf {
    u8 pad_00[4];
    u16 count;
    u16 stride;
    u8 pad_08[2];
    u8 firstEntry[1];                  /* a WMMpRecvData */
} WMMpRecvBuf;

WMMpRecvData *WM_ReadMPData(const WMMpRecvBuf *recvBuf, u16 aid);

/* A child's MP receive buffer: the frame the parent sent (the child-side
 * one WMMpRecvBuf; the WMMpRecvBuf above is its parent-side header). */
typedef struct RadioChildFrameBuffer {
    u8 pad_00[6];
    u16 length;                        /* bytes after wmHeader */
    u16 unk_08;
    u16 ackTimeStamp;
    u16 timeStamp;
    u8 rate;
    u8 rssi;                           /* raw; see Wmsp_DecodeRssi */
    u8 pad_10[8];
    u8 destAdrs[6];
    u8 srcAdrs[6];
    u8 pad_24[0xa];
    u16 pollBitmap;                    /* bit n set: the parent polls the child with aid n */
    u16 wmHeader;
    u16 data[1];
} RadioChildFrameBuffer;

/* Data sharing: every station sends a block each frame
 * and all of them receive the blocks of all. */
#define SHARE_RING_SLOTS 4
#define SHARE_SET_PAYLOAD_BYTES 508
/* WMDataSharingInfo.state values. */
#define SHARE_PHASE_IDLE 0
#define SHARE_PHASE_RUNNING 1
#define SHARE_PHASE_HALTING 2
#define SHARE_PHASE_HALTED 3
#define SHARE_PHASE_RESEND 4           /* the send queue was full; send again */
#define SHARE_PHASE_FAILED 5               /* WMDataSharingInfo.state after a failed send */
#define WIRELESS_URGENCY_RAISED 1                 /* send priority, 0 (urgent) to 3 (low) */

/* One frame's blocks, one dataLength block per aid in aidBitmap. */
typedef struct WMDataSet {
    u16 aidBitmap;                     /* stations whose block is in data */
    u16 receivedBitmap;                /* stations whose block arrived */
    u16 data[SHARE_SET_PAYLOAD_BYTES / 2];
} WMDataSet;

/* State of one data sharing session. */
typedef struct WMDataSharingInfo {
    WMDataSet ds[SHARE_RING_SLOTS];   /* ring of sets */
    u16 seqNum[SHARE_RING_SLOTS];     /* 0xffff: the set was not sent */
    u16 writeIndex;
    u16 sendIndex;
    u16 readIndex;
    u16 aidBitmap;                     /* stations taking part */
    u16 dataLength;                    /* bytes each station sends */
    u16 stationNumber;
    u16 dataSetLength;
    u16 port;
    u16 doubleMode;                    /* 1: sets are written two ahead */
    u16 currentSeqNum;
    u16 state;
    u16 reserved[1];
} WMDataSharingInfo;

/* WMStatus.wepMode */
#define WM_WEPMODE_NO 0
#define WIRELESS_CIPHER_KEY5 1
#define WIRELESS_CIPHER_KEY13 2
#define WIRELESS_CIPHER_KEY16 3

/* WMStatus.unk_1e8: the wireless bit rate. */
#define WIRELESS_BITRATE_ONE 1
#define WIRELESS_BITRATE_TWO 2

/* What WMStatus.state holds. */
#define WM_STATE_READY 0                  /* initialised, not enabled yet */
#define WM_STATE_STOP 1                    /* enabled, wireless hardware off */
#define WM_STATE_IDLE 2
#define WM_STATE_CLASS1 3
#define WM_STATE_SCAN 5
#define WM_STATE_PARENT 7
#define WM_STATE_CHILD 8
#define WM_STATE_MP_PARENT 9
#define WM_STATE_MP_CHILD 10
#define WM_STATE_DCF_CHILD 11

#define WIRELESS_OUTBOX_ENTRIES 32
#define WIRELESS_URGENCY_LEVELS 4
#define WIRELESS_OUTBOX_NO_ENTRY 0xffff           /* no entry: ends a list */

/* One port send request, an entry in the send queue. */
typedef struct RadioOutboxEntry {
    u16 next;                          /* index of the next entry in its list */
    u8 pad_02[0x1e];
} RadioOutboxEntry;

/* A list of RadioOutboxEntry entries, by index. */
typedef struct RadioOutboxList {
    u16 head;
    u16 tail;
} RadioOutboxList;

/* The ARM7 keeps these up to date; the ARM9 invalidates its cache lines
 * before each read. Only the fields read so far are named. */
typedef struct WMStatus {
    u16 state;                         /* WIRELESS_PHASE_* */
    u16 busyApiid;                     /* the request the ARM7 runs now */
    u32 apiBusy;                       /* 1 while a request runs */
    u8 pad_08[4];
    u32 mpFlag;                        /* 1 while MP runs */
    u32 unk_10;
    u32 unk_14;
    u32 dcfSendFlag;                   /* 1 while a DCF frame is being sent */
    u32 unk_1c;                        /* 1: MP sends set flag 0x8000 */
    u8 wlVersion[8];                   /* firmware version, from WL command 0x306 */
    u16 macVersion;
    u16 rfVersion;
    u16 bbpVersion[2];
    u16 unk_30;
    u16 unk_32;
    u16 unk_34;                        /* a limit set by request 35; 0 there means 16 */
    u16 unk_36;                        /* kept at or below unk_34 */
    u32 unk_38;
    u16 unk_3c;
    u16 unk_3e;
    u8 pad_40[4];
    u16 mpRecvBufSel;                  /* which of mpRecvBuf gets the next frame */
    u16 mpRecvBufSize;                 /* bytes in an MP receive buffer */
    RadioChildFrameBuffer *mpRecvBuf[2];
    const void *unk_50;                /* WM_SetMPDataToPortEx refuses this buffer */
    u16 unk_54;                        /* START_MP: the send buffer's size */
    u16 unk_56;                        /* MP ack deadline, in the wireless microsecond counter / 16 */
    OSAlarm mpAckAlarm;
    u16 mpWaitAckFlag;                 /* nonzero while an MP ack is awaited */
    u16 mpReadyBitmap;                 /* the children that take part in MP */
    u16 unk_88;
    u16 unk_8a;
    u16 unk_8c;
    u16 unk_8e;
    u16 unk_90;
    u16 unk_92;
    u16 unk_94;
    u16 unk_96;
    u16 unk_98;                        /* START_MP: defaultRetryCount */
    u16 unk_9a;
    u16 unk_9c;                        /* START_MP sets 60 */
    u8 dcfDestAdr[6];
    const u16 *dcfSendData;
    u16 dcfSendSize;
    u16 dcfRecvBufSel;
    void *dcfRecvBuf[2];               /* two halves of the buffer given to WM_StartDCF */
    u16 dcfRecvBufSize;
    u16 tgid;                          /* the parent's tgid, compared with each beacon */
    u16 linkLevel;
    u16 unk_ba;
    u16 unk_bc;                        /* START_MP sets 4 */
    u16 unk_be;
    u16 wepKeyId;
    u16 unk_c2;                        /* WM_SetMPDataToPortEx needs 1 */
    u8 pad_c4[6];
    u16 unk_ca;
    u32 unk_cc;
    u32 unk_d0;
    u32 unk_d4;
    u16 unk_d8;
    u16 unk_da;                        /* 1: a WL fatal error does not disconnect */
    u8 macAddress[6];
    u16 unk_e2;                        /* WMSP_MeasureChannel sets 2 */
    u8 pparam[0x10];                   /* start of the WMParentParam the parent runs with; unk_f4 is its maxEntry */
    u16 unk_f4;
    u8 pad_f6[0x2e];
    u8 childMacAddress[15][6];         /* [aid - 1] */
    u16 childBitmap;                   /* bit n set: the child with aid n is connected */
    u8 pad_180[4];
    u16 aid;                           /* this station's aid, 0 on a parent */
    u8 pad_186[8];
    u16 unk_18e;
    u16 unk_190;
    u16 wepMode;                       /* WIRELESS_CIPHER_* */
    u32 wepFlag;                       /* 1 while a WEP mode is set */
    u8 wepKey[80];
    u16 unk_1e8;
    u16 unk_1ea;
    u8 pad_1ec[4];
    u16 enableChannel;                 /* bit n set: channel n usable (WL command 0x283) */
    u16 allowedChannel;                /* bit 0 for channel 1; PickWirelessChannels' pick */
    u16 portSeqNo[16][8];              /* per station and port; reset to 1 */
    RadioOutboxEntry sendQueueData[WIRELESS_OUTBOX_ENTRIES];
    RadioOutboxList sendQueueFreeList;
    RadioOutboxList sendQueue[WIRELESS_URGENCY_LEVELS];
    RadioOutboxList readyQueue[WIRELESS_URGENCY_LEVELS];
    OSMutex sendQueueMutex;
    u32 unk_72c;
    u64 mpLastRecvTick[16];            /* tick of the last MP frame from each aid, bit 0 set */
    u64 unk_7b0;
} WMStatus;

/* What WIRELESS_NOTICE_HEARD_BEACON reports. */
typedef struct RadioHeardBeaconNotice {
    u16 apiid;
    u16 errcode;
    u16 state;
    u16 tgid;
    u16 wmstate;                       /* WMStatus.state */
    u16 gameInfoLength;
    WMGameInfo gameInfo;
} RadioHeardBeaconNotice;

/* Arguments of the connect command, written into WMArm9Buf.requestBuf. */
typedef struct WMStartConnectReq {
    u16 apiid;
    u8 pad_02[2];
    WMBssDesc *pInfo;                  /* the parent to join */
    u8 ssid[24];
    u32 powerSave;
    u16 reserved;
    u16 authMode;
} WMStartConnectReq;

/* Arguments of the scan command, written into WMArm9Buf.requestBuf. */
typedef struct WMStartScanReq {
    u16 apiid;
    u16 channel;
    WMBssDesc *scanBuf;
    u16 maxChannelTime;
    u8 bssid[6];
} WMStartScanReq;

/* What WM_SetParentParameter reads (0x40 bytes). */
typedef struct WMParentParam {
    u16 *userGameInfo;
    u16 userGameInfoLength;            /* at most 112 */
    u8 pad_06[2];
    u32 ggid;
    u16 tgid;
    u16 entryFlag;
    u16 maxEntry;
    u16 multiBootFlag;
    u16 KS_Flag;                       /* nonzero: key sharing, which costs payload bytes */
    u16 CS_Flag;
    u16 beaconPeriod;                  /* 10 to 1000 */
    u8 pad_1a[0x18];
    u16 channel;                       /* 1 to 14 */
    u16 parentMaxSize;
    u16 childMaxSize;
    u8 pad_38[8];
} WMParentParam;

/* Arguments of the channel measurement command, written into
 * WMArm9Buf.requestBuf. */
typedef struct WMMeasureChannelReq {
    u16 apiid;
    u16 ccaMode;
    u16 edThreshold;
    u16 channel;
    u16 measureTime;                   /* milliseconds */
} WMMeasureChannelReq;

typedef void (*WMCallbackFunc)(void *arg);

/* The ARM9 side's work area. */
typedef struct WMArm9Buf {
    void *arm7Buf;                     /* the ARM7 side's work area */
    WMStatus *status;
    u8 pad_08[4];
    void *requestBuf;                  /* where a command's arguments are written */
    void *replyBuf;                    /* where the ARM7 writes its replies */
    u16 dmaNo;
    u16 scanOnlyFlag;                  /* 0: the status block is read after each reply */
    WMCallbackFunc callbackTable[WM_NUM_OF_CALLBACK];
    WMCallbackFunc indCallback;
    WMCallbackFunc portCallback[16];
    void *portCallbackArg[16];
    u8 pad_140[0x1c];
    u32 unk_15c;                       /* Wireless_SetUpWorkArea sets 0 */
} WMArm9Buf;

/* WM internals shared by the command functions. WMi_CheckStateEx returns 0 when
 * the current state is one of the count states listed after it, else an error
 * code. func_02116df4 returns 3 (illegal state) before the WM is initialised.
 * func_02116f74 stores the callback to run when command apiid completes.
 * WMi_SendCommand sends a command: its id, the count of extra arguments, then
 * those arguments. It returns WM_ERRCODE_OPERATING once the command is
 * queued, else an error code. */
int WMi_CheckStateEx(int count, ...);
int func_02116df4(void);
int Wireless_SetUpWorkArea(void *workArea, u16 dmaNo, u32 size);
int WM_Init(void *workArea, u16 dmaNo);
int WM_Initialize(void *workArea, WMCallbackFunc callback, u16 dmaNo);
int WM_Reset(WMCallbackFunc callback);
int WM_SetIndCallback(WMCallbackFunc callback);
int WM_SetEntry(WMCallbackFunc callback, int entryFlag);
int WM_Finish(void);                    /* only while READY; clears the WM work area pointer */
int WMi_CheckIdle(void);                /* illegal state while READY or STOP */
WMArm9Buf *func_02116e10(void);
int WMi_SendCommand(int apiid, u16 argCount, ...);
int func_02116f74(int apiid, WMCallbackFunc callback);
int WM_StartMPEx(WMCallbackFunc callback, u16 *recvBuf, u16 recvBufSize, u16 *sendBuf, u16 sendBufSize,
                 u16 mpFreq, u16 defaultRetryCount, BOOL minPollBmpMode, BOOL singlePacketMode,
                 BOOL fixFreqMode, BOOL ignoreFatalError);
int WM_GetMPReceiveBufferSize(void);  /* bytes, 0 when MP cannot start */
int WM_GetMPSendBufferSize(void);
u16 *DataShare_StationBlockPtr(WMDataSharingInfo *dsInfo, u16 aidBitmap, u16 *receiveBuf, u32 aid);
u16 *WM_GetSharedDataAddress(WMDataSharingInfo *dsInfo, WMDataSet *receiveData, u16 aid);
void DataShare_PushCurrentSet(WMDataSharingInfo *dsInfo, BOOL delayed);
void DataShare_StoreStationBlock(WMDataSharingInfo *dsInfo, u16 aid, u16 *data);
BOOL Wireless_ParentSettingsValid(WMParentParam *param);
int WM_SetPortCallback(int port, WMCallbackFunc callback, void *arg);

/*
 * ARM7 side: the two parameter blocks the ARM7 main passes to the wireless
 * library's init (Wmsp_Init). Field meanings are not known yet; the
 * values look like stack addresses, stack sizes and thread priorities.
 */
typedef struct Unk_arm7_038057d4 Unk_arm7_038057d4;
struct Unk_arm7_038057d4 {
    void *unk_00;
    void *unk_04;
    u32 unk_08;
    u32 unk_0c;
    u32 unk_10;
    u32 unk_14;
    u32 unk_18;
    u32 unk_1c;
    u32 unk_20;
    u32 unk_24;
    u32 unk_28;
    void *unk_2c;
    u32 unk_30;
};

typedef struct Unk_arm7_038057d4_2 Unk_arm7_038057d4_2;
struct Unk_arm7_038057d4_2 {
    u32 unk_00;
    u32 unk_04;
    u32 unk_08;
    u32 unk_0c;
    u32 unk_10;
    u32 unk_14;
    u32 unk_18;
};

#endif
