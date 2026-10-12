#include "sdk/wmsp.h"
#include "sdk/mi.h"

/* WL command ids, named after what the handler does around them. */
#define WIFI_REQ_CHANNEL_BUSY_RATIO 10
#define WIFI_REQ_ENTER_RADIO_ON 0x302                /* WMStatus.state becomes WM_STATE_IDLE after it */
#define WIFI_REQ_ENTER_READY 0x303              /* WMStatus.state becomes WM_STATE_CLASS1 after it */
#define WIFI_REQ_READ_MODE 0x308              /* reply word 3: the WL state */

#define WIFI_REPORTED_RADIO_ON 16                      /* the WL needs setup and class 1 before measuring */

u16 *func_arm7_027c1e74(WlCommand *cmd);
u16 *func_arm7_027c1ee4(WlCommand *cmd);
u16 *func_arm7_027c1efc(WlCommand *cmd);
u16 *func_arm7_027c2890(WlCommand *cmd, int arg0, int arg1, int arg2);
u16 *func_arm7_027c2468(WlCommand *cmd, u16 arg1, u16 arg2, u16 arg3, const void *data);
void WMSP_ReplyMeasureChannelFailed(u16 wlCmdID, u16 wlResult);

void WMSP_MeasureChannel(WMMeasureChannelReq *req)
{
    WmspBuf *wmBuf = func_arm7_027c05e0()->buf;
    WlCommandBuf buf;
    WlCommand *cmd = &buf.command;
    u8 channels[16];
    WMMeasureChannelCallback *reply;
    u16 *wlReply;
    u16 wlState;
    u16 ccaMode;
    u16 edThreshold;
    u16 channel;
    u16 measureTime;
    u16 result;
    u16 measuredChannel;
    u16 busyRatio;

    if (wmBuf->status->state != WM_STATE_IDLE) {
        reply = Wmsp_GetCallbackBuffer();
        reply->apiid = WM_APIID_MEASURE_CHANNEL;
        reply->errcode = WM_ERRCODE_ILLEGAL_STATE;
        Wmsp_SendReply(reply);
        return;
    }
    wlReply = func_arm7_027c1e74(cmd);
    if (wlReply[2] != 0) {
        WMSP_ReplyMeasureChannelFailed(WIFI_REQ_READ_MODE, wlReply[2]);
        return;
    }
    wlState = wlReply[3];
    wmBuf->status->unk_e2 = 2;
    if (wlState == WIFI_REPORTED_RADIO_ON) {
        /* The original passes START_SCAN's id: a failure here replies as START_SCAN. */
        if (!WMSP_SetAllParams(WM_APIID_START_SCAN, cmd)) {
            return;
        }
        wlReply = func_arm7_027c1ee4(cmd);
        if (wlReply[2] != 0) {
            WMSP_ReplyMeasureChannelFailed(WIFI_REQ_ENTER_READY, wlReply[2]);
            return;
        }
        wmBuf->status->state = WM_STATE_CLASS1;
        wlReply = func_arm7_027c2890(cmd, 1, 0, 1);
        if (wlReply[2] != 0) {
            WMSP_ReplyMeasureChannelFailed(1, wlReply[2]);
            return;
        }
        wmBuf->status->unk_c2 = 1;
    }
    ccaMode = req->ccaMode;
    edThreshold = req->edThreshold;
    channel = req->channel;
    measureTime = req->measureTime;
    Mem_FillBytes_Arm7(channels, 0, 16);
    channels[0] = channel;
    wlReply = func_arm7_027c2468(cmd, ccaMode, edThreshold, measureTime, channels);
    if (wlReply[2] != 0) {
        WMSP_ReplyMeasureChannelFailed(WIFI_REQ_CHANNEL_BUSY_RATIO, wlReply[2]);
        return;
    }
    result = wlReply[4];
    measuredChannel = result & 0xff;
    busyRatio = result >> 8;
    wlReply = func_arm7_027c1efc(cmd);
    if (wlReply[2] != 0) {
        WMSP_ReplyMeasureChannelFailed(WIFI_REQ_ENTER_RADIO_ON, wlReply[2]);
        return;
    }
    wmBuf->status->state = WM_STATE_IDLE;
    reply = Wmsp_GetCallbackBuffer();
    reply->apiid = WM_APIID_MEASURE_CHANNEL;
    reply->errcode = WM_ERRCODE_SUCCESS;
    reply->channel = measuredChannel;
    reply->ccaBusyRatio = busyRatio;
    Wmsp_SendReply(reply);
}
