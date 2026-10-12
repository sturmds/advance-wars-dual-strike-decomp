#include "sdk/wl.h"

int WifiParam_SetAntennaChoice(int);

int func_arm7_027cdc78(WlRequest *request, WlReply *reply)
{
    reply->status = 1;
    if (WIFI_DRIVER_STATE->state.mode < 16) {
        return 1;
    }
    return WifiParam_SetAntennaChoice(request->unk_10);
}
