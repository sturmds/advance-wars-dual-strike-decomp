#include "sdk/wmsp.h"

void WMSP_QueueVAlarmMp(void);

void WMSP_VAlarmLine260(void)
{
    WmspBuf *buf = func_arm7_027c05e0()->buf;

    if (buf->status->mpFlag == 1) {
        Wmsp_SetVAlarm(buf, 209);
        WMSP_QueueVAlarmMp();
    }
}
