#include "sdk/wmsp.h"

void WMSP_AdjustVCount(void);

void WMSP_VAlarmLine208(void)
{
    WmspBuf *buf = func_arm7_027c05e0()->buf;
    WMStatus *status;

    WMSP_AdjustVCount();
    status = buf->status;
    if (status->unk_d4 >= 127) {
        status->unk_1c = 0;
    }
    Wmsp_SetVAlarm(buf, 240);
}
