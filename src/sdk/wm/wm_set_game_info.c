#include "types.h"
#include "sdk/wm.h"
#include "sdk/os.h"

void MIi_CpuCopy16(const void *src, void *dest, u32 size);

extern u8 gWmGameInfoBuf[0x80];

/* Parent side: sets the game data a beacon advertises. The data is copied
 * into a buffer the ARM7 reads. */
int WM_SetGameInfo(WMCallbackFunc callback, const void *userGameInfo, u16 length, u32 ggid,
                  u16 tgid, u8 attr)
{
    int result = WMi_CheckStateEx(2, WM_STATE_PARENT, WM_STATE_MP_PARENT);

    if (result != 0) {
        return result;
    }
    if (userGameInfo == NULL) {
        return WM_ERRCODE_INVALID_PARAM;
    }
    if (length > 112) {
        return WM_ERRCODE_INVALID_PARAM;
    }
    MIi_CpuCopy16(userGameInfo, gWmGameInfoBuf, length);
    DC_StoreRange(gWmGameInfoBuf, length);
    func_02116f74(WM_APIID_SET_GAMEINFO, callback);
    result = WMi_SendCommand(WM_APIID_SET_GAMEINFO, 5, gWmGameInfoBuf, length, ggid, tgid, attr);
    if (result == 0) {
        result = WM_ERRCODE_OPERATING;
    }
    return result;
}
