#include "sdk/wmsp.h"

void WMSP_CalcFrameOffset(void);

void WMSP_VAlarmLine200(void)
{
    WmspBuf *buf = func_arm7_027c05e0()->buf;
    WMStatus *status;

    buf->status->unk_cc = WMSP_FRAME_START_TIME;
    status = buf->status;
    if (status->unk_d0 != status->unk_cc) {
        status->unk_d0 = status->unk_cc;
        WMSP_CalcFrameOffset();
    }
    status = buf->status;
    if (status->unk_d4 > 127) {
        Wmsp_SetVAlarm(buf, 208);
    } else {
        status->unk_1c = 1;
        Wmsp_SetVAlarm(buf, 240);
    }
}
