#include "sdk/wl.h"

int WifiParam_SetAntennaSwitching(int, int);

int func_arm7_027cdc30(WlRequest *request, WlReply *reply)
{
    reply->status = 1;
    if (WIFI_DRIVER_STATE->state.mode < 16) {
        return 1;
    }
    return WifiParam_SetAntennaSwitching(request->unk_10, request->unk_12);
}
