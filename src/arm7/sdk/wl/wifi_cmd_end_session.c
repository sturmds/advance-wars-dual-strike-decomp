#include "sdk/wl.h"

void Wifi_CloseSession(void); /* stops the current session: mode 32, interrupts off */

int WifiCmd_EndSession(WlStopSessionRequest *request, WlReply *reply)
{
    reply->status = 1;
    if (request->clearCounters > 1) {
        return WIFI_ANSWER_OUT_OF_RANGE;
    }
    Wifi_CloseSession();
    if (request->clearCounters == 1) {
        WifiStats_Zero();
    }
    return WIFI_ANSWER_OK;
}
