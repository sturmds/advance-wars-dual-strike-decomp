#include "sdk/wmsp.h"

void Wmsp_BuildGameInfo(WMGameInfo *info, const WMParentParam *param);
u16 *func_arm7_027c1f70(WlCommand *cmd, int size, const void *data);

/* Needed to match: a ternary over enum constants puts moveq first. The original
   WMErrCode is an enum; wm.h has the values as #defines. */
enum { ERRCODE_SUCCESS = WM_ERRCODE_SUCCESS, ERRCODE_FAILED = WM_ERRCODE_FAILED };

/* Bytes of WMGameInfo before userGameInfo. */
#define GAME_INFO_HEADER_SIZE 16

/* WMStatus.pparam holds a WMParentParam; wm.h declares it as bytes. Needed to
   match: cast at each use, a pointer local changes the code. */
#define PPARAM(status) ((WMParentParam *)(status)->pparam)

void WMSP_SetGameInfo(WmspSetGameInfoRequest *request)
{
    WMStatus *status = func_arm7_027c05e0()->buf->status;
    u32 wlBuf[WMSP_WL_BUF_WORDS];
    WMGameInfo gameInfo;
    u8 attribute;
    u16 *wlReply;
    WmspReply *reply;

    PPARAM(status)->userGameInfo = request->userGameInfo;
    PPARAM(status)->userGameInfoLength = request->userGameInfoLength;
    PPARAM(status)->ggid = request->ggid;
    PPARAM(status)->tgid = request->tgid;
    attribute = request->attribute;
    PPARAM(status)->entryFlag = (attribute & WM_ATTR_FLAG_ENTRY) ? 1 : 0;
    PPARAM(status)->multiBootFlag = (attribute & WM_ATTR_FLAG_MB) ? 1 : 0;
    PPARAM(status)->KS_Flag = (attribute & WIRELESS_BEACON_BIT_KEYSHARE) ? 1 : 0;
    PPARAM(status)->CS_Flag = (attribute & WIRELESS_BEACON_BIT_STREAMING) ? 1 : 0;

    Wmsp_BuildGameInfo(&gameInfo, PPARAM(status));
    wlReply = func_arm7_027c1f70((WlCommand *)wlBuf, (u16)(PPARAM(status)->userGameInfoLength + GAME_INFO_HEADER_SIZE),
                                 &gameInfo);

    reply = Wmsp_GetCallbackBuffer();
    reply->apiid = WM_APIID_SET_GAMEINFO;
    reply->errcode = wlReply[2] == 0 ? ERRCODE_SUCCESS : ERRCODE_FAILED;
    if (wlReply[2] != 0) {
        reply->wlCmdID = 0x245;
        reply->wlResult = wlReply[2];
    }
    Wmsp_SendReply(reply);
}
