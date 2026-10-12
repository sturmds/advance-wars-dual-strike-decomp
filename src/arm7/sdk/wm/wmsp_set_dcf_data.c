#include "sdk/wmsp.h"
#include "sdk/mi.h"

u16 *func_arm7_027c23b0(WlCommand *cmd, WmspTxFrame *frame);

/* Needed to match: the original stores the zero on the stack and loads it back. */
static inline void MI_CpuClear16(void *dest, u32 size)
{
    volatile u16 zero = 0;

    Mem_FillHalfwords_Arm7(zero, dest, size);
}

/* Needed to match: a ternary over enum constants puts moveq first. The original
   WMErrCode is an enum; wm.h has the values as #defines. */
enum { ERRCODE_SUCCESS = WM_ERRCODE_SUCCESS, ERRCODE_FAILED = WM_ERRCODE_FAILED };

void WMSP_SetDCFData(WmspSetDCFDataRequest *request)
{
    WMStatus *status = func_arm7_027c05e0()->buf->status;
    WmspTxFrame frame;
    u32 wlBuf[WMSP_WL_BUF_WORDS];
    u16 *wlReply;
    WmspReply *reply;

    Mem_CopyBytes_Arm7(request->destAdr, status->dcfDestAdr, 6);
    status->dcfSendData = request->data;
    status->dcfSendSize = request->size;
    status->dcfSendFlag = 1;

    MI_CpuClear16(&frame, sizeof(frame));
    frame.status = 0;
    frame.length = request->size;
    frame.rate = status->unk_1e8 == WIRELESS_BITRATE_TWO ? 20 : 10;
    Mem_CopyBytes_Arm7(request->destAdr, frame.destAdr, 6);
    Mem_CopyBytes_Arm7(status->macAddress, frame.srcAdr, 6);
    frame.data = request->data;
    wlReply = func_arm7_027c23b0((WlCommand *)wlBuf, &frame);

    reply = Wmsp_GetCallbackBuffer();
    reply->apiid = WM_APIID_SET_DCF_DATA;
    reply->errcode = wlReply[2] == 0 ? ERRCODE_SUCCESS : ERRCODE_FAILED;
    if (wlReply[2] != 0) {
        reply->wlCmdID = 0x100;
        reply->wlResult = wlReply[2];
    }
    Wmsp_SendReply(reply);
}
