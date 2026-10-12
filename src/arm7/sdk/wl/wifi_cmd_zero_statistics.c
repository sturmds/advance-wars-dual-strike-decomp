#include "sdk/wl.h"

void func_arm7_027c9030(void); /* updates, then clears WlWork.counters */

int WifiCmd_ZeroStatistics(WlRequest *request, WlReply *reply)
{
    if (WIFI_DRIVER_STATE->state.mode == 0) {
        return 1;
    }
    reply->status = 1;
    func_arm7_027c9030();
    return 0;
}
