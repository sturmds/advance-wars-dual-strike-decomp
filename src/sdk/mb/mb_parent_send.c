#include "types.h"
#include "sdk/mb.h"
#include "sdk/wm.h"

extern DlPlayStationArea *data_022aced8;

void func_0211c794(void);
int MbParent_SetMpData(void (*callback)(void), void *data, u16 size, u16 unused, u16 bitmap);

/* Sends data to the children in bitmap with Wireless_QueueMpFrameData. Returns 0 when the
 * send was queued, 1 when nothing was sent, else the WM error code. The u16
 * locals are needed to match. */
int MbParent_SendMpData(void *data, int size, int bitmap)
{
    DlPlayStationArea *work = data_022aced8;
    u16 size16 = size;
    u16 bitmap16 = bitmap;
    int result;

    if (work->unk_528 == 0 || work->unk_526 == 1) {
        return 1;
    }
    switch (work->unk_524) {
    case 1:
        result = MbParent_SetMpData(work->sendCallback, data, size16, work->unk_52c == 0 ? 1000 : 0, bitmap16);
        if (result == WM_ERRCODE_OPERATING) {
            data_022aced8->sendQueued = 1;
        }
        return result == WM_ERRCODE_OPERATING ? 0 : result;
    case 2:
        result = MbParent_SetMpData(func_0211c794, data, size16, 0, bitmap16);
        if (result == WM_ERRCODE_OPERATING) {
            data_022aced8->sendQueued = 1;
        }
        return result == WM_ERRCODE_OPERATING ? 0 : result;
    default:
        return 1;
    }
}
