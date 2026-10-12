#include "sdk/wmsp.h"

/* WL command ids, named after what the handler does around them. */
#define WIFI_REQ_FIND_PARENTS 2
#define WIFI_REQ_ENTER_READY 0x303              /* WMStatus.state becomes WM_STATE_CLASS1 after it */
#define WIFI_REQ_READ_MODE 0x308              /* reply word 3: the WL state */

#define WIFI_REPORTED_RADIO_ON 16                      /* the WL needs setup and class 1 before scanning */

u16 *func_arm7_027c1e74(WlCommand *cmd);
u16 *func_arm7_027c1ee4(WlCommand *cmd);
u16 *func_arm7_027c2890(WlCommand *cmd, int arg0, int arg1, int arg2);
u16 *func_arm7_027c27d4(WlCommand *cmd, u32 size, const void *mac, int arg0, const void *block32, u16 arg1,
                        const void *block16, u16 arg2);
int func_arm7_027c3408(int a0);
u8 Wmsp_DecodeRssi(int rawRssi);
int Wmsp_RssiToLinkLevel(u32 rssi);

/* Allowed in WM_STATE_IDLE, WM_STATE_CLASS1 and WM_STATE_SCAN: scans the
   channels in the request's list and reports every parent found. Their
   beacons go one after the other into scanBuf, each at a 4-byte boundary. */
void WMSP_StartScanEx(RadioChannelSweepRequest *req)
{
    union {
        WlCommand command;
        u8 bytes[0x45c];
    } cmdBuf;
    /* Needed to match: this declaration order (it sets which callee-saved
       register each local gets). */
    int i;
    WMBssDesc *desc;
    WMBssDesc *dest;
    WlCommand *cmd = &cmdBuf.command;
    u16 bssid[3];
    u8 ssid[32];
    u8 channels[16];
    volatile u16 zero;
    RadioChannelSweepReply *reply;
    WmspScanReply *scan;
    u16 *result;
    u16 channelList;
    u16 count;
    WmspBuf *buf = func_arm7_027c05e0()->buf;
    int channelMask;
    u16 maxChannelTime;
    u16 scanType;
    u16 ssidLength;
    u16 scanBufSize;
    u16 channel;
    u16 size;

    if (buf->status->state != WM_STATE_IDLE && buf->status->state != WM_STATE_CLASS1 &&
        buf->status->state != WM_STATE_SCAN) {
        reply = Wmsp_GetCallbackBuffer();
        reply->apiid = WM_APIID_START_SCAN_EX;
        reply->errcode = WM_ERRCODE_ILLEGAL_STATE;
        reply->state = WM_STATECODE_PARENT_NOT_FOUND;
        Wmsp_SendReply(reply);
        return;
    }
    buf->status->pInfoBuf = req->scanBuf;
    channelList = req->channelList;
    buf->status->scanChannel = channelList;
    maxChannelTime = req->maxChannelTime;
    Mem_CopyBytes_Arm7(req->bssid, bssid, sizeof(bssid));
    scanType = req->scanType;
    ssidLength = req->ssidLength;
    Mem_CopyBytes_Arm7(req->ssid, ssid, sizeof(ssid));
    scanBufSize = req->scanBufSize;
    /* all 0xff scans for any parent; a scan for one clears the group bit */
    if (bssid[0] != 0xffff && (bssid[0] & 1)) {
        bssid[0] &= ~1;
    }
    /* from here bit n stands for channel n */
    channelMask = (u16)(channelList << 1);
    channelMask &= buf->status->enableChannel;
    if (channelMask == 0) {
        reply = Wmsp_GetCallbackBuffer();
        reply->apiid = WM_APIID_START_SCAN_EX;
        reply->errcode = WM_ERRCODE_INVALID_PARAM;
        reply->state = WM_STATECODE_PARENT_NOT_FOUND;
        Wmsp_SendReply(reply);
        return;
    }
    if (req->scanBuf == NULL || ((u32)req->scanBuf & 3) != 0 || req->scanBufSize < 64) {
        reply = Wmsp_GetCallbackBuffer();
        reply->apiid = WM_APIID_START_SCAN_EX;
        reply->errcode = WM_ERRCODE_INVALID_PARAM;
        reply->state = WM_STATECODE_PARENT_NOT_FOUND;
        Wmsp_SendReply(reply);
        return;
    }
    buf->status->unk_e2 = 2;
    result = func_arm7_027c1e74(cmd);
    if (result[2] != 0) {
        WMSP_ReplyStartScanFailed(WIFI_REQ_READ_MODE, result[2], TRUE);
        return;
    }
    if (result[3] == WIFI_REPORTED_RADIO_ON) {
        if (!WMSP_SetAllParams(WM_APIID_START_SCAN_EX, cmd)) {
            return;
        }
        result = func_arm7_027c1ee4(cmd);
        if (result[2] != 0) {
            WMSP_ReplyStartScanFailed(WIFI_REQ_ENTER_READY, result[2], TRUE);
            return;
        }
        buf->status->state = WM_STATE_CLASS1;
        result = func_arm7_027c2890(cmd, 1, 0, 1);
        if (result[2] != 0) {
            WMSP_ReplyStartScanFailed(1, result[2], TRUE);
            return;
        }
        buf->status->unk_c2 = 1;
    }
    buf->status->state = WM_STATE_SCAN;
    count = 0;
    Mem_FillBytes_Arm7(channels, 0, sizeof(channels));
    for (channel = 1; channel < 15; channel++) {
        if (channelMask & (1 << channel)) {
            channels[count] = channel;
            count++;
        }
    }
    scan = (WmspScanReply *)func_arm7_027c27d4(cmd, func_arm7_027c3408(scanBufSize), bssid, ssidLength, ssid,
                                               scanType, channels, maxChannelTime);
    if (scan->status != 0) {
        WMSP_ReplyStartScanFailed(WIFI_REQ_FIND_PARENTS, scan->status, TRUE);
        return;
    }
    reply = Wmsp_GetCallbackBuffer();
    if (scan->bssCount == 0) {
        reply->apiid = WM_APIID_START_SCAN_EX;
        reply->errcode = WM_ERRCODE_SUCCESS;
        reply->state = WM_STATECODE_PARENT_NOT_FOUND;
        reply->bssDescCount = 0;
        reply->channelList = channelMask >> 1;
    } else {
        desc = (WMBssDesc *)&scan->bssLength;
        dest = buf->status->pInfoBuf;
        zero = 0;
        Mem_FillHalfwords_Arm7(zero, dest, scanBufSize);
        for (i = 0; i < scan->bssCount; i++) {
            size = desc->length * 2;
            Mem_CopyBytes_Arm7(desc, dest, size);
            reply->bssDesc[i] = dest;
            reply->linkLevel[i] = Wmsp_RssiToLinkLevel(Wmsp_DecodeRssi((u8)desc->rssi));
            desc = (WMBssDesc *)((u8 *)desc + size);
            dest = (WMBssDesc *)((u8 *)dest + size);
            if ((u32)dest & 2) {
                dest = (WMBssDesc *)(((u32)dest + 2) & ~3);
            }
        }
        reply->apiid = WM_APIID_START_SCAN_EX;
        reply->errcode = WM_ERRCODE_SUCCESS;
        reply->state = WM_STATECODE_PARENT_FOUND;
        reply->bssDescCount = scan->bssCount;
        reply->channelList = channelMask >> 1;
    }
    Wmsp_SendReply(reply);
}
