#include "sdk/wl.h"

int WifiCmd_ReportSsidHidden(WlRequest *request, WlReply *reply)
{
    reply->status = 2;
    reply->value = WIFI_DRIVER_STATE->state.flags.bit0;
    return 0;
}
