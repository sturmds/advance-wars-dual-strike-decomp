#include "sdk/gx.h"
#include "sdk/wmsp.h"

void WMSP_StoreFrameStartTime(void);

void WMSP_VAlarmLine209(void)
{
    WmspBuf *buf = func_arm7_027c05e0()->buf;
    WMStatus *status = buf->status;
    int vcount;

    /* unk_d8 counts frames: once every 60 the line counter is written back to itself */
    if (status->unk_d8 >= 60) {
        vcount = *REG_VCOUNT;
        if (vcount >= 209 && vcount < 212) {
            *REG_VCOUNT = vcount;
            buf->status->unk_d8 = 0;
        }
    } else {
        status->unk_d8++;
    }
    WMSP_StoreFrameStartTime();
    Wmsp_SetVAlarm(buf, 260);
}
