#include "sdk/wl.h"

int WifiParam_SetBeaconForwarding(int);

int func_arm7_027cdbe8(WlRequest *request, WlReply *reply)
{
    reply->status = 1;
    if (WIFI_DRIVER_STATE->state.mode < 16) {
        return 1;
    }
    return WifiParam_SetBeaconForwarding(request->unk_10);
}
