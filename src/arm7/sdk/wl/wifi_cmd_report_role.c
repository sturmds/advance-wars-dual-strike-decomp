#include "sdk/wl.h"

int WifiCmd_ReportRole(WlRequest *request, WlReply *reply)
{
    reply->status = 2;
    reply->value = WIFI_DRIVER_STATE->state.phase;
    return 0;
}
