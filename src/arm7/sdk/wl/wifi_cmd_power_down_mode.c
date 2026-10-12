#include "sdk/wl.h"

void func_arm7_037f9380(int mode); /* switches WlLink.mode */

int WifiCmd_PowerDownMode(WlRequest *request, WlReply *reply)
{
    reply->status = 1;
    if (WIFI_DRIVER_STATE->state.mode == 0 || WIFI_DRIVER_STATE->state.mode == WIFI_MODE_RADIO_ON) {
        func_arm7_037f9380(0);
        return 0;
    }
    return 1;
}
