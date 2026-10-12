#include "sdk/wl.h"

void func_arm7_037f92c8(void); /* stops the current session: mode 32, interrupts off */
void Wifi_RestartDriver(void); /* register defaults, then Wifi_ClearSessionArea */

int WifiCmd_ResetDriver(WlRequest *request, WlReply *reply)
{
    reply->status = 1;
    if (WIFI_DRIVER_STATE->state.mode >= 32) {
        func_arm7_037f92c8();
    }
    Wifi_RestartDriver();
    return 0;
}
