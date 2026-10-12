#include "sdk/wl.h"

int WifiCmd_ReportListenPeriod(WlRequest *request, WlReply *reply)
{
    reply->status = 2;
    reply->value = WIFI_DRIVER_STATE->state.unk_8c;
    return 0;
}
