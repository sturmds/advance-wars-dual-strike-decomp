#include "sdk/wl.h"

int WifiCmd_ReportStationCapacity(WlRequest *request, WlReply *reply)
{
    reply->status = 2;
    // WifiCmd_SetStationCapacity stores the request value plus one.
    reply->value = WIFI_DRIVER_STATE->limits.unk_f6 - 1;
    return 0;
}
