#include "sdk/wmsp.h"

/* Starts the MP V-count alarm chain: at line 209 when unk_e2 is 1, at line 200 when it is 2. */
void WMSP_StartMpVAlarm(void)
{
    WmspBuf *buf = func_arm7_027c05e0()->buf;
    WMStatus *status = buf->status;

    if (status->unk_e2 == 1) {
        if (buf->vAlarm.handler != NULL) {
            LineAlarm_Withdraw_Arm7(&buf->vAlarm);
        }
        Wmsp_SetVAlarm(buf, 209);
    } else if (status->unk_e2 == 2) {
        status->unk_1c = 0;
        if (buf->vAlarm.handler != NULL) {
            LineAlarm_Withdraw_Arm7(&buf->vAlarm);
        }
        Wmsp_SetVAlarm(buf, 200);
        buf->status->unk_d4 = 0;
    }
}
