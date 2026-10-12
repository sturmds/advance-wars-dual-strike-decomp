#include "sdk/wmsp.h"

#define WIFI_REQ_TIMEOUT_SETUP 0x211

/* 100 ms in cycles of the 33.514 MHz system clock. One tick is 64 cycles. */
#define CYCLES_PER_100MS 3351400

u16 *func_arm7_027c2124(WlCommand *cmd, u16 arg0, u16 arg1, u16 arg2);
void WMSP_ReplySetLifeTimeFailed(u16 wlCmdID, u16 wlResult);

void WMSP_SetLifeTime(WmspSetLifeTimeRequest *request)
{
    WMStatus *status = func_arm7_027c05e0()->buf->status;
    u16 mpLifeTime = request->mpLifeTime;
    WlCommandBuf buf;
    WmspReply *reply;
    u16 *wlReply;
    u64 now;
    int aid;

    wlReply = func_arm7_027c2124(&buf.command, request->tgid, request->camLifeTime,
                                 request->frameLifeTime);
    if (wlReply[2] != 0) {
        WMSP_ReplySetLifeTimeFailed(WIFI_REQ_TIMEOUT_SETUP, wlReply[2]);
        return;
    }
    if (mpLifeTime != 0xffff) {
        /* needed to match: a 64-bit 1, not a 32-bit one widened afterwards */
        status->unk_7b0 = mpLifeTime == 0
            ? (s64)1
            : _s32_div_f(mpLifeTime * CYCLES_PER_100MS, 64);
    } else {
        status->unk_7b0 = 0;
    }
    now = Tick_Read64_Arm7() | 1;
    for (aid = 0; aid < 16; aid++) {
        status->mpLastRecvTick[aid] = now;
    }
    reply = Wmsp_GetCallbackBuffer();
    reply->apiid = WM_APIID_SET_LIFETIME;
    reply->errcode = WM_ERRCODE_SUCCESS;
    Wmsp_SendReply(reply);
}
