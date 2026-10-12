#include "sdk/wl.h"

int WifiCmd_ReportAntennaChoice(WlRequest *request, WlReply *reply)
{
    reply->status = 2;
    if (WIFI_DRIVER_STATE->state.mode < 16) {
        return 1;
    }
    reply->value = WIFI_DRIVER_STATE->state.flags.bit3;
    return 0;
}
