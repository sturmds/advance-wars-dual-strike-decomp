#include "types.h"
#include "sdk/mi.h"
#include "sdk/wl.h"

u8 WL_ReadByte(const void *addr);
void WL_WriteByte(void *addr, u32 value);
void func_arm7_027c8594(void);                       /* cancels the one-shot alarm */
void func_arm7_027c90ec(u16 *dest, const u16 *src);  /* copies a 6-byte MAC address */
void func_arm7_027d1d84(WlElements *elements);

extern const u16 gWlZeroMacAddr[3];

/* WlScanJob.state values (see WifiMlme_RunScan). */
#define WIFI_SCAN_STATE_LISTENING 0x13
#define WIFI_SCAN_STATE_DONE 0x15
/* Below this many free halfwords in the reply the scan ends early. */
#define WIFI_SCAN_REPLY_SPARE_MIN 32

/* Body bytes before the elements: timestamp, beacon interval, capability. */
#define WIFI_BEACON_BODY_HEAD_BYTES 12
/* Bytes of WlScanEntry before gameInfo. */
#define WIFI_SCAN_RESULT_HEAD_BYTES 0x40U
/* Halfwords an entry takes: the fixed fields, then the game info, rounded up. */
#define WIFI_SCAN_RESULT_HALFWORDS(gameInfoLength) ((WIFI_SCAN_RESULT_HEAD_BYTES + (gameInfoLength) + 1) / 2)

/*
 * Beacon or probe response heard while WifiMlme_RunScan listens on a channel.
 * A sender whose BSSID is in the request's list only sets its bit in
 * reply->value. Any other network is added once to the reply, if its SSID
 * passes the filter and the entry fits; the scan ends when the reply is
 * nearly full. elements is NULL when the caller has not parsed the frame's
 * elements yet.
 */
void WifiScan_RecordHeardNetwork(WlRxFrame *frame, WlElements *elements)
{
    volatile u16 zero16; /* needed to match: the fill values go through the stack */
    volatile u32 zero32;
    WlElements parsed;
    WlWork *work = WIFI_DRIVER_STATE;
    WlScanJob *job = &WIFI_ACTIVE_JOB_OF(work)->scan;
    WlReply *reply;
    WlMgmtBody *body;
    WlScanEntry *entry;
    /* needed to match: i before bssid */
    u32 i;
    u16 *bssid;
    u16 length;

    if (job->state != WIFI_SCAN_STATE_LISTENING) {
        return;
    }
    reply = job->reply;
    body = (WlMgmtBody *)frame->body;
    bssid = job->request->bssids[0];
    for (i = 0; i < job->request->unk_4c; bssid += 3, i++) {
        if (WifiMac_Equal(bssid, frame->addr3)) {
            reply->value |= 1 << i;
            return;
        }
    }
    entry = (WlScanEntry *)&reply->words[1];
    for (i = 0; i < reply->words[0]; i++) {
        if (WifiMac_Equal(frame->addr3, entry->bssid)) {
            return;
        }
        entry = (WlScanEntry *)((u16 *)entry + entry->length);
    }
    zero16 = 0;
    Mem_FillHalfwords_Arm7(zero16, entry, WIFI_SCAN_RESULT_HEAD_BYTES + 4);
    length = frame->bodyLength;
    if (length <= WIFI_BEACON_BODY_HEAD_BYTES) {
        return;
    }
    if (elements == NULL) {
        zero32 = 0;
        Mem_FillWords_Arm7(zero32, &parsed, sizeof(parsed));
        parsed.data = body->probeResponse.elements;
        parsed.length = length - WIFI_BEACON_BODY_HEAD_BYTES;
        parsed.result = 3;
        parsed.unk_0c = 0x38;
        parsed.unk_04 = frame->unk_08;
        parsed.capability = body->probeResponse.capability;
        func_arm7_027d1d84(&parsed);
        elements = &parsed;
    }
    if (elements->nintendo != NULL) {
        entry->gameInfoLength = WL_ReadByte(elements->nintendo + 1) - 8;
    }
    if ((elements->result & WIFI_IE_FITS_SSID) == WIFI_IE_FITS_SSID && WIFI_ACTIVE_JOB_OF(WIFI_DRIVER_STATE)->scan.unk_04 >= WIFI_SCAN_RESULT_HALFWORDS(entry->gameInfoLength)) {
        entry->capability = body->probeResponse.capability;
        func_arm7_027c90ec(entry->bssid, frame->addr3);
        entry->beaconInterval = body->probeResponse.beaconInterval;
        entry->rssi = frame->unk_12 & 0xff;
        if (elements->nintendo != NULL) {
            for (i = 0; i < entry->gameInfoLength; i++) {
                WL_WriteByte(entry->gameInfo + i, WL_ReadByte(elements->nintendo + 10 + i));
            }
        }
        if (elements->unk_18 != NULL) {
            entry->ssidLength = WL_ReadByte(elements->unk_18 + 1);
            for (i = 0; i < entry->ssidLength; i++) {
                WL_WriteByte(entry->ssid + i, WL_ReadByte(elements->unk_18 + 2 + i));
            }
        } else {
            entry->ssidLength = 0;
            for (i = 0; i < WIFI_SSID_LONGEST; i++) {
                WL_WriteByte(entry->ssid + i, 0);
            }
        }
        entry->unk_2e = elements->unk_14;
        entry->supportedRates = elements->supportedRates;
        entry->unk_36 = elements->unk_12;
        if (elements->unk_1c != NULL) {
            entry->unk_38 = WL_ReadByte(elements->unk_1c + 3);
        }
        if (elements->unk_20 != NULL) {
            entry->unk_34 = WL_ReadByte(elements->unk_20 + 3);
        }
        entry->length = WIFI_SCAN_RESULT_HALFWORDS(entry->gameInfoLength);
        reply->status += entry->length;
        reply->words[0]++;
        job->unk_04 -= entry->length;
        if (job->unk_04 < WIFI_SCAN_REPLY_SPARE_MIN) {
            func_arm7_027c8594();
            job->state = WIFI_SCAN_STATE_DONE;
            WifiTask_Schedule(2, 0);
        }
    } else {
        func_arm7_027c90ec(entry->bssid, gWlZeroMacAddr);
    }
}
