#include "sdk/wl.h"

int WifiCmd_ReportShortPreamble(WlRequest *request, WlReply *reply)
{
    reply->status = 2;
    reply->value = WIFI_DRIVER_STATE->state.flags.bit2;
    return 0;
}
