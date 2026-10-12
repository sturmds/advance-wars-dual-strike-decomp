#include "sdk/wl.h"

int WifiCmd_ReportPostBeaconTime(WlRequest *request, WlReply *reply)
{
    reply->status = 2;
    reply->value = WIFI_DRIVER_STATE->state.unk_10;
    return 0;
}
