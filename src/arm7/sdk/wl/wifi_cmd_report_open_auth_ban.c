#include "sdk/wl.h"

int WifiCmd_ReportOpenAuthBan(WlRequest *request, WlReply *reply)
{
    reply->status = 2;
    reply->value = WIFI_DRIVER_STATE->state.unk_06;
    return 0;
}
