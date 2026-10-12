#include "sdk/wmsp.h"

void func_arm7_027c5ab0(void);

void WMSP_VAlarmLine208(void)
{
    WmspBuf *buf = func_arm7_027c05e0()->buf;
    WMStatus *status;

    func_arm7_027c5ab0();
    status = buf->status;
    if (status->unk_d4 >= 127) {
        status->unk_1c = 0;
    }
    Wmsp_SetVAlarm(buf, 240);
}
