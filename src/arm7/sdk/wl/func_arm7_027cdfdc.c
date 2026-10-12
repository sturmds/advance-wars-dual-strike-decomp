#include "sdk/wl.h"

int WifiParam_StoreAllowedChannels(int);

int func_arm7_027cdfdc(WlRequest *request, WlReply *reply)
{
    reply->status = 1;
    if (WIFI_DRIVER_STATE->state.mode != 16) {
        return 1;
    }
    return WifiParam_StoreAllowedChannels(request->unk_10);
}
