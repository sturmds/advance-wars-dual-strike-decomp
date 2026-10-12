#include "sdk/wl.h"

int WifiCmd_ReportRetryLimit(WlRequest *request, WlReply *reply)
{
    reply->status = 2;
    reply->value = WIFI_DRIVER_STATE->unk_2fe;
    return 0;
}
