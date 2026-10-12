#include "sdk/wl.h"

int WifiCmd_ReportAntennaSwitching(WlRequest *request, WlReply *reply)
{
    reply->status = 3;
    if (WIFI_DRIVER_STATE->state.mode < 16) {
        return 1;
    }
    reply->value = WIFI_DRIVER_STATE->state.flags.bit4;
    reply->words[0] = WIFI_DRIVER_STATE->state.flags.bit5 ^ WIFI_DRIVER_STATE->state.flags.bit3;
    return 0;
}
