#ifndef SDK_WM_H
#define SDK_WM_H

#include "types.h"

/* What the callbacks passed to func_02117cac and its siblings receive. The
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
#define WM_STATECODE_PARENT_NOT_FOUND 4
#define WM_STATECODE_PARENT_FOUND 5
#define WM_STATECODE_CONNECT_START 6
#define WM_STATECODE_CONNECTED 7
#define WM_STATECODE_BEACON_LOST 8
#define WM_STATECODE_DISCONNECTED 9
#define WM_STATECODE_MP_START 10
#define WM_STATECODE_MPEND_IND 11
#define WM_STATECODE_MP_IND 12
#define WM_STATECODE_PORT_RECV 21

/* WMCallback.errcode values. */
#define WM_ERRCODE_SUCCESS 0
#define WM_ERRCODE_TIMEOUT 9
#define WM_ERRCODE_INVALID_POLLBITMAP 13
#define WM_ERRCODE_SEND_FAILED 15

#define WM_ATTR_FLAG_ENTRY 0x01  /* the parent accepts new children */
#define WM_ATTR_FLAG_MB 0x02     /* the parent offers a download play game */

/* What an indication callback receives (unsolicited ARM7 events). */
typedef struct RadioNotice {
    u16 apiid;
    u16 errcode;
    u16 state;                         /* WIRELESS_NOTICE_* */
} RadioNotice;

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
} WMStartMPCallback;

/* Start of the reply to the connect command; only these fields are read. */
typedef struct WMStartConnectCallback {
    u16 apiid;
    u16 errcode;
    u16 wlCmdID;
    u16 wlResult;
    u16 state;
    u16 aid;                           /* the id the parent gave this child */
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
} WMPortRecvCallback;

/* One child's data in an MP receive buffer; length 0xffff or 0 means none
 * arrived. Only these fields are read so far. */
typedef struct WMMpRecvData {
    u16 length;
    u16 unk_02;
    u16 aid;                           /* the sender */
} WMMpRecvData;

/* Receive buffer of an MP exchange: count entries, stride bytes apart. */
typedef struct WMMpRecvBuf {
    u8 pad_00[4];
    u16 count;
    u16 stride;
    u8 pad_08[2];
    u8 firstEntry[1];                  /* a WMMpRecvData */
} WMMpRecvBuf;

/* What WMStatus.state holds. */
#define WM_STATE_IDLE 2
#define WM_STATE_CLASS1 3
#define WM_STATE_SCAN 5
#define WM_STATE_PARENT 7
#define WM_STATE_CHILD 8
#define WM_STATE_MP_PARENT 9
#define WM_STATE_MP_CHILD 10
#define WM_STATE_DCF_CHILD 11

/* The ARM7 keeps these up to date; the ARM9 invalidates its cache lines
 * before each read. Only the fields read so far are named. */
typedef struct WMStatus {
    u16 state;                         /* WIRELESS_PHASE_* */
    u8 pad_02[0x0a];
    u32 unk_0c;
    u8 pad_10[0xa8];
    u16 linkLevel;
    u8 pad_ba[0x3a];
    u16 unk_f4;
    u8 pad_f6[0x88];
    u16 childBitmap;                   /* bit n set: the child with aid n is connected */
    u8 pad_180[4];
    u16 unk_184;
    u8 pad_186[8];
    u16 unk_18e;
    u16 unk_190;
} WMStatus;

/* The ARM9 side's work area. */
typedef struct WMArm9Buf {
    u8 pad_00[4];
    WMStatus *status;
    u8 pad_08[4];
    void *requestBuf;                  /* where a command's arguments are written */
    u8 pad_10[0xac];
    void *indCallback;
    void *portCallback[16];
    void *portCallbackArg[16];
} WMArm9Buf;

/* WM internals shared by the command functions. func_02116cf0 returns 0 when
 * the current state is one of the count states listed after it, else an error
 * code. func_02116df4 returns 3 (illegal state) before the WM is initialised.
 * func_02116e20 sends the command with its argument count, 0 on failure. */
int func_02116cf0(int count, ...);
int func_02116df4(void);
WMArm9Buf *func_02116e10(void);

#endif
