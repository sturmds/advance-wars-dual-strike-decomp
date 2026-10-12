#include "sdk/wl.h"

u16 func_arm7_037fd434(void);
void func_arm7_037fd424(OSAlarm *alarm);

void Wifi_CreateAlarms(void)
{
    WlWork *work = WIFI_DRIVER_STATE;

    if (!func_arm7_037fd434()) {
        work->state.unk_12 |= 0x40;
    } else {
        func_arm7_037fd424(&work->periodicAlarm);
        func_arm7_037fd424(&work->oneShotAlarm);
    }
}
