#include "sdk/mi.h"
#include "sdk/wl.h"

void func_arm7_027c8e34(void); /* adds the hardware event counters into WlWork.counters */

int WifiCmd_ReportStatistics(WlRequest *request, WlReply *reply)
{
    if (WIFI_DRIVER_STATE->state.mode == 0) {
        return 1;
    }
    reply->status = 92;
    func_arm7_027c8e34();
    Mem_CopyWords_Arm7(WIFI_DRIVER_STATE->counters, reply->words, sizeof(WIFI_DRIVER_STATE->counters));
    return 0;
}
