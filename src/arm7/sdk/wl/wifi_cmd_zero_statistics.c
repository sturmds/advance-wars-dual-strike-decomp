#include "sdk/wl.h"

int WifiCmd_ZeroStatistics(WlRequest *request, WlReply *reply)
{
    if (WIFI_DRIVER_STATE->state.mode == 0) {
        return 1;
    }
    reply->status = 1;
    WifiStats_Zero();
    return 0;
}
