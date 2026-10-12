#include "sdk/wl.h"

int WifiCmd_ReportWepKeyIndex(WlRequest *request, WlReply *reply)
{
    reply->status = 2;
    reply->value = WIFI_DRIVER_STATE->state.unk_0a;
    return 0;
}
