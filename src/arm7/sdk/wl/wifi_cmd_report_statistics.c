#include "sdk/mi.h"
#include "sdk/wl.h"

int WifiCmd_ReportStatistics(WlRequest *request, WlReply *reply)
{
    if (WIFI_DRIVER_STATE->state.mode == 0) {
        return 1;
    }
    reply->status = 92;
    WifiStats_Accumulate();
    Mem_CopyWords_Arm7(WIFI_DRIVER_STATE->counters, reply->words, sizeof(WIFI_DRIVER_STATE->counters));
    return 0;
}
