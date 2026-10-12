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
/* Station states below WIFI_PEER_ASSOCIATED, as the frame handlers set them. */
#define WIFI_PEER_KNOWN_ONLY 0x20 /* set on a deauthentication */
#define WIFI_PEER_AUTH_DONE 0x30   /* association requests are refused below this */

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

// Request of WifiCmd_ArmMpReply.
typedef struct WlMpReplyRequest {
    u8 pad_00[0x10];
    u16 length; /* bytes at data, at most 0x204 */
    u16 header; /* first body halfword, before the data */
    const void *data;
} WlMpReplyRequest;

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
#define WIFI_MODE_JOINED 64 /* set when an association response reports success */
#define WIFI_MODE_READY 32     /* entered by Wifi_CloseSession */

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

/* 802.11 rates as bit masks, one bit per rate (bit 0 1 Mbit/s, bit 1 2 Mbit/s, ...). */
typedef struct WlRateSet {
    u16 basic;
    u16 supported;
} WlRateSet;

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
    u8 pad_0a[2];
    u16 unk_0c;
    u16 unk_0e;
    u16 unk_10;     /* set to 1 on each pre-beacon interrupt */
    u16 unk_12;
    u16 unk_14;
    u16 testRate;   /* 10 or 20, from the WifiCmd_RunRadioTest request */
    u16 testTxActive;
    u16 unk_1a;
    u8 pad_1c[2];
    u16 ssidLength;
    u8 ssid[32];
    u8 pad_40[0x60 - 0x40];
    u16 basicRates;     /* bit n: rate n of the rate table is a basic rate */
    u16 supportedRates; /* bit n: rate n of the rate table is supported */
    u16 bssid[3];
    u16 aid;            /* passed to WifiTx_BuildPsPoll */
    u8 pad_6c[0x6e - 0x6c];
    u16 beaconInterval; /* sent in probe responses */
    u16 listenInterval; /* sent in association requests */
    u16 unk_72;
    u16 unk_74;
    u16 unk_76;          /* counts down from unk_74 - 1 once per beacon */
    u8 pad_78[0x7c - 0x78];
    u16 flags_7c;
    u16 unk_7e;          /* WlState.unk_96: pre-beacons between runs of task 13 */
    u16 unk_80;          /* WlState.unk_98: pre-beacons counted towards unk_7e */
    u16 currentAp[3];   /* MAC address sent in reassociation requests */
    u16 unk_88; /* station index of data frames sent outside phase 1 */
    u16 flags_8a;
    u16 unk_8c;                    /* WlState.unk_a4 */
    u8 pad_8e[4];
    u16 ssidOffset;                /* SSID element in the tx frame payload */
    u8 pad_94[0x96 - 0x94];
    u16 gameInfoOffset; /* Nintendo element offset in the beacon, same halfword as WlState.unk_ae */
    u8 pad_98[0x9c - 0x98];
    u8 *gameInfo;       /* sent in the Nintendo element of the beacon */
    u16 gameInfoLength;
    u16 unk_a2;         /* bit 0: gameInfo starts one byte later */
    u16 unk_a4;         /* 1 once gameInfo changes, 0 once the beacon has it */
    u8 pad_a6[2];
    u32 periodicCount;  /* WlState.periodicCount */
    u16 savedBbReg6; /* baseband register 6 before WifiTest_SendPatternFrames changed it */
    u8 pad_ae[2];
    u16 pendingEvents; /* WlState.unk_c8: event bits task 21 sends to the host */
    u8 pad_b2[0xb8 - 0xb2];
    u16 unk_b8;        /* WlState.unk_d0 */
    u16 wepRestarts;   /* times WifiIrq_TxErrorCounted restarted the WEP engine */
} WlLink;

// Entry inside the frame payload at a running offset.
typedef struct WlPayloadEntry {
    u8 pad_00[6];
    u8 bytes[2];
} WlPayloadEntry;

/*
 * A frame as the wireless hardware reads it from WIFI_PACKET_RAM: a 12-byte TX header,
 * then the 802.11 header, then the body.
 */
typedef struct WlRamFrame {
    u16 status;
    u16 unk_02;
    u16 unk_04;
    u16 unk_06;
    u16 rate;         /* WIFI_RATE_CODE_* */
    u16 length;       /* 802.11 header, body and FCS */
    u16 frameControl;
    u16 duration;     /* the AID in a PS-Poll */
    u16 addr1[3];
    u16 addr2[3];
    u16 addr3[3];
    u16 sequence;
    u16 body[1];
} WlRamFrame;

/* An MP reply frame in WIFI_PACKET_RAM and whether it waits to be sent. */
typedef struct WlMpReplySlot {
    u16 busy;
    u16 count;    /* frames set up in this slot */
    u8 pad_04[4];
    WlRamFrame *frame;
    struct WlTxFrame *txFrame; /* the frame as the driver built it */
    u8 pad_10[4];
} WlMpReplySlot;

/*
 * The seven TX queue records at WlState offset 0x100, seen as one array
 * (WIFI_SEND_QUEUE_RECORDS_OF). Record 2's frame is WlState.psPollFrame, records 4 and 5
 * are WlState.mpReplySlots, record 6 starts at WlState.unk_178.
 */
typedef struct WlTxQueue {
    u16 busy;
    u8 pad_02[6];
    WlRamFrame *frame;
    u8 *unk_0c;     /* 16 bytes into a block func_arm7_027d0ee8 reads */
    u8 pad_10[4];
} WlTxQueue;

#define WIFI_SEND_QUEUE_RECORDS_OF(work) ((WlTxQueue *)(work)->state.pad_100)

/* The two halfwords kept just before each queued frame in WIFI_PACKET_RAM; a frame
   found without them is rewritten as a 1-byte frame with them as header. */
#define WIFI_GUARD_WORD_A 0xb6b8
#define WIFI_GUARD_WORD_B 0x1d46

// Frame buffer the driver builds; the payload starts at 0x24.
typedef struct WlTxFrame {
    u16 unk_00;
    u16 station;
    u16 unk_04;         /* low half of periodicCount when the frame was queued */
    u16 unk_06;
    u16 result; /* set when the frame is done, returned in the reply */
    u8 pad_0a[2];
    u16 unk_0c;         /* WlRamFrame.unk_04 of the copy that was sent */
    u16 requestedRate;  /* 0: use the station's rate */
    u16 rate;           /* WIFI_RATE_CODE_* */
    u16 unk_12;
    u16 frameControl;
    u16 duration;
    u16 addr1[3];
    u16 addr2[3];
    union {
        u8 payload[10];
        struct {
            u16 addr3[3];
            u16 sequence;
            u16 body;
        };
    };
    u16 unk_2e;
} WlTxFrame;

/*
 * A received frame: 8 bytes the driver fills, the hardware RX header from
 * 0x08, the 802.11 header from 0x14, then the body.
 */
typedef struct WlRxFrame {
    u16 unk_00;
    u16 station;        /* station table index, 0 when the sender is unknown */
    u16 unk_04;
    u16 bodyLength;
    u16 unk_08;         /* first word of the hardware RX header */
    u8 pad_0a[0x10 - 0x0a];
    u16 length;         /* 802.11 header and body */
    u16 unk_12;
    u16 frameControl;
    u16 duration;
    u16 addr1[3];
    u16 addr2[3];
    u16 addr3[3];
    union {
        u16 sequence;
        struct {
            u16 fragment : 4;
            u16 sequenceNumber : 12;
        };
    };
    u16 body[1];
} WlRxFrame;

/* 802.11 frame control bits. */
#define WIFI_FRAMECTL_SENT_TO_AP 0x100
#define WIFI_FRAMECTL_SENT_BY_AP 0x200
#define WIFI_FRAMECTL_FRAGMENTS_FOLLOW 0x400

/* Destination, source and sequence of a received data frame, worked out from
   the To DS and From DS bits. */
typedef struct WlRxAddrs {
    u16 dest[3];
    u16 src[3];
    u16 sequence;
} WlRxAddrs;

/* 802.11 frame control, as bitfields. */
typedef union WlFrameControl {
    u16 raw;
    struct {
        u16 version : 2;
        u16 type : 2; /* 0: management frame */
        u16 subtype : 4;
        u16 toDs : 1;
        u16 fromDs : 1;
        u16 moreFragments : 1;
        u16 retry : 1;
        u16 powerManagement : 1;
        u16 moreData : 1;
        u16 protectedFrame : 1;
        u16 order : 1;
    };
} WlFrameControl;

/* A u16 frameControl field read through its bitfields. */
#define WIFI_FRAMECTL_FIELDS(field) ((WlFrameControl *)&(field))
#define WIFI_FRAMECTL_WEP_ON 0x4000

/* WlTxFrame.result of a frame taken off a queue unsent. */
#define WIFI_SEND_RESULT_UNSENT 2

/*
 * One hardware TX location (LOC1, LOC2, LOC3, CMD, two MP reply slots, the
 * beacon) and the frame it sends. TX queue n feeds slot n.
 */
typedef struct WlTxSlot {
    u16 busy;
    u16 count;
    u16 sentCount;
    u8 pad_06[2];
    WlRamFrame *frame;   /* the frame in WIFI_PACKET_RAM */
    WlTxFrame *queued;   /* the queued frame it was copied from */
    void (*onDone)(WlTxFrame *queued, int arg);
} WlTxSlot;

#define WIFI_TXLOC_DATA 0
#define WIFI_TXLOC_MGMT 1
#define WIFI_TXLOC_GROUP_OR_PSPOLL 2 /* frame: WlState.psPollFrame */
#define WIFI_TXLOC_MP_POLL 3  /* busy: WlState.unk_13c */
#define WIFI_TXLOC_POLL_ANSWER_A 4 /* WlState.mpReplySlots; which is which is a guess */
#define WIFI_TXLOC_POLL_ANSWER_B 5
#define WIFI_TXLOC_BEACON 6 /* busy: WlState.unk_178, frame: WlState.txFrame */

// The MLME request in progress, in WlState.unk_d8 (work offset 0x3d8; reach
// it with WIFI_ACTIVE_JOB_OF). Its handler runs again
// as a WL task until state goes back to 0, then it fills the reply. Each kind
// of request keeps its own fields after state.

typedef struct WlMeasureChannelRequest {
    u8 pad_00[0x12];
    u16 reg13Value;
    u16 reg35Value;
    u16 sampleMs;      /* time spent on each channel */
    u8 channels[16];   /* 0 ends the list */
} WlMeasureChannelRequest;

typedef struct WlMeasureChannelJob {
    u16 state;
    u8 pad_02[2];
    u32 sampleCount;
    u32 busyTotal;     /* 100 for each sample that found the channel busy */
    u16 savedReg040;
    u16 savedReg13;
    u16 savedReg35;
    u8 pad_12[2];
    u16 index;
    u16 result;
    WlMeasureChannelRequest *request;
    WlReply *reply;
} WlMeasureChannelJob;

/* Request of the authenticate, associate and reassociate steps. */
typedef struct WlPeerRequest {
    u8 pad_00[0x10];
    u16 peer[3];       /* address handed to the frame builder */
    u16 authAlgorithm; /* authenticate only */
    u16 timeoutMs;
} WlPeerRequest;

typedef struct WlPeerJob {
    u16 state;
    u8 pad_02[0x16];
    WlPeerRequest *request;
    WlReply *reply;
} WlPeerJob;

/* Authentication frame from WifiFrame_NewAuthentication: the 802.11 header starts at
   0x14 (frame control 0xb0), the body at 0x2c. */
typedef struct WlAuthFrame {
    u8 pad_00[2];
    u16 station;         /* as in WlTxFrame and WlRxFrame */
    u8 pad_04[0x28];
    u16 algorithm;
    u16 sequence;
    u16 status;
    u8 challengeId;      /* challenge text element, shared key only */
    u8 challengeLength;
    u16 challenge[1];
} WlAuthFrame;

/* Request checked and applied by func_arm7_027cc97c. */
typedef struct WlBssSettings {
    u8 pad_00[0x10];
    u16 ssidLength;      /* 1-32 */
    u8 ssid[32];
    u16 unk_32;          /* 10-1000 */
    u16 unk_34;          /* 1-255 */
    u16 channel;
    u16 unk_38[2];       /* nonzero 12-bit masks */
    u16 unk_3c;          /* length of unk_3e, at most 128 */
    u8 unk_3e[128];
} WlBssSettings;

/* Scan request (task 0, WifiMlme_RunScan). */
typedef struct WlScanRequest {
    u8 pad_00[0x10];
    u16 peer[3];         /* address handed to the probe request builder */
    u16 ssidLength;      /* up to 32 */
    u8 ssid[32];
    u16 unk_38;          /* nonzero: send no probe requests */
    u8 channels[16];     /* 0 ends the list */
    u16 channelTimeMs;   /* time spent on each channel */
    u16 unk_4c;          /* up to 16 */
} WlScanRequest;

typedef struct WlStopSessionRequest {
    u8 pad_00[0x10];
    u16 clearCounters; /* 0 or 1 */
} WlStopSessionRequest;

/* Reply that carries the WlUnk318 data block. */
typedef struct WlDataReply {
    u16 unk_00;
    u16 length;     /* reply length in halfwords: room on entry, used on return */
    u16 unk_04;
    u16 dataLength; /* in bytes */
    u8 data[1];
} WlDataReply;

typedef struct WlScanJob {
    u16 state;
    u8 pad_02[2];
    u16 unk_04;          /* WlReply.status - 2 when the scan was asked for */
    u16 channelIndex;    /* next entry of request->channels */
    u16 unk_08;
    u16 elapsedMs;       /* time spent on the current channel */
    u16 probeIntervalMs;
    u8 pad_0e[0xa];
    WlScanRequest *request;
    WlReply *reply;
} WlScanJob;

/* Task 1 (func_arm7_027cc5ac): waits for the receive path to fill result. */
typedef struct WlTask1Request {
    u8 pad_00[0x10];
    u16 timeoutMs;
} WlTask1Request;

typedef struct WlTask1Job {
    u16 state;
    u8 pad_02[2];
    u16 result;
    u16 unk_06;
    u8 pad_08[0x10];
    WlTask1Request *request;
    WlReply *reply;
} WlTask1Job;

/* Values of WlReply.unk_04 set by the MLME steps. */
#define WIFI_ANSWER_OK 0
#define WIFI_ANSWER_ROOM_SHORT 4
#define WIFI_ANSWER_OUT_OF_RANGE 5
#define WIFI_ANSWER_NO_RESPONSE 7
#define WIFI_ANSWER_NO_MEMORY 8  /* the frame could not be allocated */
#define WIFI_ANSWER_FLASH_BLOCK_BAD 14  /* WifiFlash_ValidateRadioBlock failed */
#define WIFI_ANSWER_LATER 0x80 /* the request stays in WlMlme; the reply comes later */

/* Request that names a destination station by MAC address. Bit 0 of the
   first byte marks a multicast address. */
typedef struct WlMacRequest {
    u8 pad_00[0x10];
    u16 mac[3];
    u16 reason; /* 802.11 reason code */
} WlMacRequest;

/* Reply that echoes the station address of a WlMacRequest. */
typedef struct WlMacReply {
    u16 unk_00;
    u16 status;
    u16 unk_04;
    u16 mac[3];
} WlMacReply;

/* Request to (re)associate with the access point at mac. */
typedef struct WlAssociateRequest {
    u8 pad_00[0x10];
    u16 mac[3];
    u16 listenInterval; /* 1-255 */
    u16 timeoutMs;      /* 10-2000: how long to wait for the response */
} WlAssociateRequest;

typedef struct WlMlmeJob {
    u16 state;
    u8 pad_02[0x16];
    void *request;
    WlReply *reply;
} WlMlmeJob;

typedef union WlMlme {
    u16 state;
    WlMlmeJob any;
    WlMeasureChannelJob measure;
    WlPeerJob peer;
    WlTask1Job task1;
    WlScanJob scan;
} WlMlme;

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
    u16 unk_0c;
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
    u16 unk_2c;
    u8 pad_2e[8];
    u16 unk_36;
    u16 words[16];
    u16 words_58[16];
    u16 unk_78;
    u16 unk_7a;
    u16 unk_7c[3];
    u16 aid; /* association ID */
    u8 pad_84[0x2];
    u16 unk_86;
    u16 unk_88;
    u8 pad_8a[2];
    u16 unk_8c;
    u8 pad_8e[4];
    u16 unk_92;
    u16 capability; /* same halfword as WlLink.flags_7c */
    u16 unk_96;
    u16 unk_98;
    u8 pad_9a[0xa];
    u16 unk_a4;
    u8 pad_a6[0xac - 0xa6];
    /* Wi-Fi RAM offset of the beacon frame that holds the TIM element. */
    u16 timOffset;
    u16 unk_ae;
    u8 pad_b0[0xb2 - 0xb0];
    u16 ringSize;                  /* length of the MAC RAM ring that ends at WIFI_RX_RING_LIMIT */
    void *unk_b4;
    u16 unk_b8;                    /* same halfword as WlLink.gameInfoLength */
    u8 pad_ba[0xbe - 0xba];
    u16 unk_be;
    u32 periodicCount;             /* periodic alarm firings */
    u8 pad_c4[4];
    u16 unk_c8;
    u8 pad_ca[0xce - 0xca];
    u16 unk_ce;                    /* counts frames found with their marks overwritten */
    u16 unk_d0;                    /* counts TX queue restarts */
    u8 pad_d2[0xd8 - 0xd2];
    u16 unk_d8[16];
    WlCurrentRequest current;
    u8 pad_100[0x130 - 0x100];
    struct WlRamFrame *psPollFrame;
    u8 pad_134[0x13c - 0x134];
    u16 unk_13c;
    u8 pad_13e[0x150 - 0x13e];
    WlMpReplySlot mpReplySlots[2];
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
    u16 unk_a4;
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

/* A buffer holding a frame to send. */
typedef struct WlTxBuffer {
    WlBuffer header;
    u16 unk_0c;
    u8 pad_0e[2];
    WlTxFrame frame;
} WlTxBuffer;

/* Longest received frame (802.11 header and body) WL handles. */
#define WIFI_RX_FRAME_CAP 0x5fc

/* A buffer holding a received frame. */
typedef struct WlRxBuffer {
    WlBuffer header;
    u16 command;
    u16 length;
    WlRxFrame frame;
} WlRxBuffer;

/* The WlRxBuffer that holds a received frame. */
#define WIFI_RX_HOLDER_OF(rxFrame) ((WlRxBuffer *)((u8 *)(rxFrame) - (u32)&((WlRxBuffer *)0)->frame))

/* WlTxBuffer.unk_0c of a frame the driver builds for itself. */
#define WIFI_DRIVER_BUILT_FRAME 0xffff

/* 802.11 management frame subtypes, as WlTxFrame.frameControl. */
#define WIFI_FRAMECTL_ASSOC_ASK 0x00
#define WIFI_FRAMECTL_ASSOC_ANSWER 0x10
#define WIFI_FRAMECTL_REASSOC_ASK 0x20
#define WIFI_FRAMECTL_REASSOC_ANSWER 0x30
#define WIFI_FRAMECTL_PROBE_ASK 0x40
#define WIFI_FRAMECTL_PROBE_ANSWER 0x50
#define WIFI_FRAMECTL_DISASSOCIATION 0xa0
#define WIFI_FRAMECTL_AUTHENTICATION 0xb0
#define WIFI_FRAMECTL_DEAUTHENTICATION 0xc0

/* 802.11 header (24 bytes) and frame check sequence (4 bytes): WlTxFrame.unk_12
   is the body length WlTxFrame.unk_06 plus this. */
#define WIFI_HEADER_PLUS_CHECKSUM 28

/* 802.11 element ID of the challenge text in shared-key authentication. */
#define WIFI_CHALLENGE_IE_ID 16

/* Bits 14-15 of an AID field, always set. */
#define WIFI_ASSOC_ID_TOP_BITS 0xc000

/* 802.11 status code of an association response when no AID is left. */
#define WIFI_IEEE_STATUS_AP_FULL 19

/* Body of a management frame the driver builds, from WlTxFrame.body. */
typedef union WlMgmtBody {
    struct {
        u16 algorithm;
        u16 sequence;
        u16 status;
        u8 elements[2];
    } auth;
    struct {
        u8 timestamp[8];
        u16 beaconInterval;
        u16 capability;
        u8 elements[2];
    } probeResponse;
    struct {
        u16 capability;
        u16 listenInterval;
        u8 elements[2];
    } assocRequest;
    struct {
        u16 capability;
        u16 listenInterval;
        u16 currentAp[3];
        u8 elements[2];
    } reassocRequest;
    struct {
        u16 capability;
        u16 status;
        u16 aid;
        u8 elements[2];
    } assocResponse; /* also reassociation responses */
    u16 reason;      /* deauthentication and disassociation */
    u8 elements[2];  /* probe requests */
} WlMgmtBody;

/* A WlTxFrame seen as a management frame, to reach its body by offset. */
typedef struct WlMgmtFrame {
    u8 pad_00[0x2c];
    WlMgmtBody body;
} WlMgmtFrame;

#define WIFI_MANAGEMENT_FIELDS_OF(frame) (&((WlMgmtFrame *)(frame))->body)

// Host buffer that carries a frame to send. The reply sits replyOffset
// halfwords after the start of frame.
typedef struct WlTxRequest {
    WlBuffer header;
    u16 command;
    u16 replyOffset;
    WlTxFrame frame;
} WlTxRequest;

/* The buffer header in front of a frame that sits in a WlTxRequest. */
#define WIFI_HOST_TX_HEADER_OF(frame) ((WlBuffer *)((u8 *)(frame) - (u32)&((WlTxRequest *)0)->frame))

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
    WlBufferList lists[8]; /* list ids 3 to 10; lists[2] is WlWork.multicastQueue */
    WlBufferList outbox;
    WlBufferList inbox;
} WlBufferPool;

/* Tasks run by the driver thread, one at a time. They wait in four FIFO
   queues by priority (0 first); next links the entries of a queue. */
#define WIFI_TASK_LIST_END 0xffff
#define WIFI_TASK_LEVELS 4
#define WIFI_TASK_LEVEL_IDLE 3
#define WIFI_TASK_SLOTS 24

typedef struct WlTask {
    u16 next;
    u16 queued;
    void (*handler)(void);
} WlTask;

/* WlWork.counters seen field by field: running totals of the hardware's 8-bit
   statistic counters (IO_WIRELESS_TALLY_BYTES). Each unk_XX field adds one counter byte. */
typedef struct WlCounters {
    u8 pad_00[0xc];
    u32 unk_0c;
    u8 pad_10[0x10];
    u32 unk_20;
    u8 pad_24[0xc];
    u32 unk_30;
    u8 pad_34[4];
    u32 unk_38;
    u32 unk_3c;
    u32 unk_40;
    u32 unk_44;
    u32 unk_48;
    u32 unk_4c;
    u32 unk_50;
    u32 unk_54;
    u32 unk_58;
    u32 unk_5c;
    u32 unk_60;
    u8 pad_64[4];
    u32 retries;      /* sum of the low bytes of WlRamFrame.unk_04 */
    u32 sentFirstTry; /* frames whose low byte was 0 */
    u8 pad_70[8];
    u32 cmdStat[15]; /* counter bytes 0x21-0x2f: 0x048081d1-0x048081df */
} WlCounters;

/* WlWork.counters index of the LOC3 (multicast) TX done count. */
#define WIFI_STAT_GROUP_SENT 5

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
    u8 pad_484[0x4a2 - 0x484];
    u16 aidBit; /* 1 << state.aid */
    u8 pad_4a4[0x4ba - 0x4a4];
    u16 unk_4ba; /* ORed into the RX header word of an MP ack sent to the host */
    u8 pad_4bc[0x504 - 0x4bc];
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
/* Frame to send in reply to the next MP poll: bit 15 enables it,
   the low bits give its WIFI_PACKET_RAM halfword offset. */
#define WIFI_TX_NEXT_POLL_ANSWER (*(volatile u16 *)0x04808094)
#define WIFI_POST_BEACON_TIMER (*(volatile u16 *)0x04808134)
#define WIFI_MAC_MODE_AND_WEP (*(volatile u16 *)0x04808006)
#define WIFI_MAC_RETRY_LIMIT (*(volatile u16 *)0x0480802c)
#define WIFI_OWN_MAC_REGISTERS ((u16 *)0x04808018)
#define WIFI_BSSID_REGISTERS ((u16 *)0x04808020) /* GBATEK */
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

/* WlState.unk_d8 (0x20 bytes) as WifiTask_BeginShutdown reads it: a halfword at 0 and
   a pointer at 0x1c. */
typedef struct Unk_arm7_027c7f84 {
    u8 pad_00[4];
    u16 unk_04;
} Unk_arm7_027c7f84;

typedef struct WlStateD8 {
    u16 unk_00;
    u8 pad_02[0x1a];
    Unk_arm7_027c7f84 *unk_1c;
} WlStateD8;

#define WIFI_JOB_AREA_OF(work) ((WlStateD8 *)(work)->state.unk_d8)

#define WIFI_LINK_VIEW_OF(work) ((WlLink *)(work)->state.pad_18)
#define WIFI_ACTIVE_JOB_OF(work) ((WlMlme *)(work)->state.unk_d8)

// Third view of the work area, starting at work offset 0x2f0 (the station
// table), as some functions address it.
typedef struct WlStateView {
    u8 pad_00[8];
    u16 macAddress[3];
    u16 unk_2fe;
    WlState state;
} WlStateView;

#define WIFI_FROM_STATION_TABLE_OF(work) ((WlStateView *)&(work)->limits.unk_f0)

// View from work offset 0x150 (the buffer allocator), as WifiTx_EnqueueHostData
// addresses its buffer lists.
typedef struct WlBufferLists {
    WlBufferAllocator allocator;
    WlBufferList unk_15c;
    WlBufferList txQueues[2]; /* WlWork.txQueues */
    u8 pad_30[0x84 - 0x30];
    WlBufferList inbox;
} WlBufferLists;

#define WIFI_LISTS_VIEW_OF(work) ((WlBufferLists *)&(work)->allocator)

// View from work offset 0x400, as WifiCmd_ArmMpReply addresses the MP reply slots
// (WlState.mpReplySlots).
typedef struct WlMpReplyView {
    WlMpReplySlot txSlots[3];
    u8 pad_3c[0x50 - 0x3c];
    WlMpReplySlot slots[2];
    u8 pad_78[0x80 - 0x78];
    WlTxFrame *txFrame;  /* WlState.txFrame */
    u8 pad_84[0x8c - 0x84];
    u32 unk_8c;
    u8 pad_90[0xb0 - 0x90];
    u16 txTimeouts;      /* TX slot frames dropped by WifiIrq_BeaconSent */
} WlMpReplyView;

#define WIFI_REPLY_SLOTS_VIEW_OF(work) ((WlMpReplyView *)((u8 *)(work) + 0x400))

/* Work offset 0x400 on: all seven TX slots, then the buffer the CMD slot
   sends. It runs past WlState. */
typedef struct WlTxSlots {
    WlTxSlot slot[7];
    u8 pad_8c[4];
    WlBuffer *cmdBuffer;
} WlTxSlots;

#define WIFI_TX_LOCATIONS_OF(work) ((WlTxSlots *)(work)->state.pad_100)
/* One slot, its offset from work in one constant (as some code adds it). */
#define WIFI_TX_LOCATION_OF(work, n) ((WlTxSlot *)&(work)->state.pad_100[(n) * sizeof(WlTxSlot)])
/* The beacon slot on its own, from WlState.unk_178. */
#define WIFI_BEACON_TX_SLOT_OF(work) ((WlTxSlot *)&(work)->state.unk_178)

/* TX queues 0-2 in a row: txQueues[0], txQueues[1], multicastQueue. */
#define WIFI_SEND_QUEUE_OF(work, n) (&(work)->txQueues[0] + (n))
#define WIFI_SEND_QUEUE_TOTAL 3
#define WIFI_SEND_QUEUE_GROUP 2

/* The request buffer of a queued frame (frame sits 16 bytes in). */
#define WIFI_HOST_TX_REQUEST_OF(frame) ((WlTxRequest *)((u8 *)(frame) - 16))

#define WIFI_DRIVER_STATE (*(WlWork **)0x0380fff4)
/* Halfword WL copies into the Nintendo beacon element after WlState.unk_10. */
#define WIFI_SHARED_GAME_WORD (*(u16 *)0x0380fff0)

/* WlState from 0x18 on as a block of its own: some code addresses these
   fields from a pointer to 0x18, so the original nests a struct there.
   ssidLength is WlState.unk_36, ssid is WlState.words, aid is WlState.aid. */
typedef struct WlParams {
    u8 pad_00[0x1e];
    u16 ssidLength;
    u8 ssid[32];
    u8 ssidMask[32]; /* a set bit matches any SSID bit */
    u8 pad_60[0xa];
    u16 aid;
    u8 pad_6c[0x1c];
    u16 unk_88;
    u8 pad_8a[0x12];
    u8 *unk_9c;  /* buffer of up to 129 bytes */
    u16 unk_a0;  /* length stored with unk_9c */
    u16 unk_a2;
    u16 unk_a4;
} WlParams;

/* 0x318: offsetof(WlWork, state) + 0x18, written as one constant so the
   compiler adds it in one step as the original does. */
#define WIFI_SETTINGS_BLOCK_OF(work) ((WlParams *)((u8 *)(work) + 0x318))

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
        u16 events; /* command 0x186 */
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
#define REG_W_RANDOM    (*(volatile u16 *)0x04808044) /* hardware random number */

/* Wi-Fi transmit request registers: writing a bit clears or sets that request. */
#define REG_W_TXREQ_RESET (*(volatile u16 *)0x048080ac)
#define REG_W_TXREQ_SET (*(volatile u16 *)0x048080ae)
#define W_TXREQ_LOC1 0x1
#define W_TXREQ_CMD 0x2
#define W_TXREQ_LOC2 0x4
#define W_TXREQ_LOC3 0x8
/* TX locations: WIFI_PACKET_RAM halfword offset of a frame, bit 15 enables it. */
#define REG_W_TXBUF_BEACON (*(volatile u16 *)0x04808080)
#define REG_W_TXBUF_LOC3 (*(volatile u16 *)0x048080a8)
#define W_TXBUF_ENABLE 0x8000
/* Writing a bit stops that TX location: the W_TXREQ_* bits, and the reply slots. */
#define REG_W_TXBUF_RESET (*(volatile u16 *)0x048080b4)
#define W_TXBUF_RESET_REPLY2 0x40
#define W_TXBUF_RESET_REPLY1 0x80
/* Wi-Fi RF serial status: bit 0 set while a transfer to the RF chip runs. */
#define REG_W_RF_BUSY (*(volatile u16 *)0x04808180)
/* Wi-Fi WEP engine control: bit 15 enables it. */
#define REG_W_WEP_CNT (*(volatile u16 *)0x04808032)

/* Wireless interrupt causes: bits of WIFI_IRQ_PENDING_FLAGS (W_IF, set) and WIFI_IRQ_ENABLE_MASK
   (W_IE, enabled). Names from GBATEK. */
#define W_IRQ_RX_DONE        0x0001
#define W_IRQ_TX_DONE        0x0002
#define W_IRQ_RX_COUNT       0x0004 /* a receive event counter went up */
#define W_IRQ_TX_COUNT       0x0008 /* a transmit error counter went up */
#define W_IRQ_COUNT_OVERFLOW 0x0030 /* RX and TX counters half full */
#define W_IRQ_RX_START       0x0040
#define W_IRQ_TX_START       0x0080
#define W_IRQ_RF_WAKEUP      0x0800
#define W_IRQ_MP_END         0x1000 /* multiplay command finished */
#define W_IRQ_POST_BEACON    0x2000
#define W_IRQ_BEACON         0x4000
#define W_IRQ_PRE_BEACON     0x8000

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

/* Work offsets 0x2f0-0x317 seen as one struct (Wifi_ClearSessionArea clears them in
   one go); func_arm7_027cc97c reaches phase and macAddress through it. */
typedef struct WlHead {
    WlStationTable stationTable;
    u16 macAddress[3];   /* WlWork.macAddress */
    u16 unk_0e;
    u16 unk_10;
    u16 phase;           /* WlState.phase */
    u8 pad_14[8];
    u16 unk_1c;                    /* WlState.unk_0c */
    u8 pad_1e[2];
    u16 unk_20;                    /* WlState.unk_10 */
} WlHead;

#define WIFI_SESSION_HEAD_OF(work) ((WlHead *)&(work)->limits.unk_f0)

typedef struct WlAuthenticateReply {
    u16 unk_00;
    u16 status;
    u16 unk_04;
    u16 unk_06;
    u16 mac[3];
    u16 algorithm;
} WlAuthenticateReply;

/* Request to join the BSS it describes (fields as in a beacon). */
typedef struct WlJoinRequest {
    u8 pad_00[0x10];
    u16 timeoutMs;      /* up to 2000 */
    u8 pad_12[6];
    u16 bssid[3];
    u16 ssidLength;     /* 1-32 */
    u8 ssid[WIFI_SSID_LONGEST];
    u16 capability;     /* 802.11 capability field */
    WlRateSet rates;    /* bits 0-11 */
    u16 beaconInterval; /* 10-1000 */
    u16 dtimPeriod;     /* up to 255 */
    u16 channel;        /* 0-15, and allowed in WlState.unk_00 */
} WlJoinRequest;

/* TX header rate codes, in 100 kbit/s. */
#define WIFI_RATE_CODE_1M 10
#define WIFI_RATE_CODE_2M 20

// Wireless baseband (BB) and RF chip serial ports.
#define IO_WIRELESS_BASEBAND_SELECT (*(volatile u16 *)0x04808158)   /* index | 0x5000 write, | 0x6000 read */
#define IO_WIRELESS_BASEBAND_OUT (*(volatile u16 *)0x0480815a)
#define IO_WIRELESS_BASEBAND_IN (*(volatile u16 *)0x0480815c)
#define IO_WIRELESS_RADIO_WORD_HI (*(volatile u16 *)0x0480817c)  /* high half of the RF word */
#define IO_WIRELESS_RADIO_WORD_LO (*(volatile u16 *)0x0480817e)  /* low half; writing DATA2 starts the transfer */
// 8-bit statistic counters, two per halfword: 0x048081b0-0x048081c1 and 0x048081d0-0x048081df.
#define IO_WIRELESS_TALLY_BYTES ((volatile u16 *)0x048081b0)
#define IO_WIRELESS_STATION_ID_4BIT (*(volatile u16 *)0x04808028)
#define IO_WIRELESS_STATION_ID (*(volatile u16 *)0x0480802a)
#define WIFI_MAC_WAKE_CONTROL (*(volatile u16 *)0x04808038)
#define WIFI_MAC_POWER_AUX (*(volatile u16 *)0x04808048)

BOOL WifiRf_AwaitSerialIdle(void);
// Returns 0 once the RF port is idle, 1 on timeout.
int WifiRf_SendWord(u32 value);
// Returns 0, or 0xffffffff when WifiBaseband_AwaitReady times out.
u32 WifiBaseband_Put(u32 index, u32 value);
u16 WifiBaseband_Get(u32 index);
// Nonzero when bit `bit` of WIFI_DRIVER_STATE->state.unk_00 is set.
u32 WifiParam_ChannelAllowed(u32 bit);
// a and b are 6-byte MAC addresses, read as three halfwords.
BOOL WifiMac_Equal(const u16 *a, const u16 *b);
void WifiStats_Accumulate(void);
void WifiStats_Zero(void);
void WifiLink_DropAssocId(void);
void WifiLink_TakeAssocId(u16 aid);
void WifiMac_CopyAddressPair(u16 *dest, const u16 *addr1, const u16 *addr2);
void WifiMac_CopyAddressTriple(u16 *dest, const u16 *addr1, const u16 *addr2, const u16 *addr3);

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
void WifiBufList_ReleaseAll(WlBufferList *list);

/* Event bits for WifiHost_RaiseEventBits. */
#define WIFI_EVT_NOTE_DROPPED 0x1 /* an indication was dropped: no buffer left */
#define WIFI_EVT_TX_BUILD_FAILED 0x2 /* a frame to send could not be built: no buffer left */
void WifiHost_RaiseEventBits(u16 events);
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
void WifiTx_DrainQueue(int queue, BOOL toHost);
void WifiTx_HaltSlot(int index);
void WifiTx_ReportSlotResult(int index);

#endif
