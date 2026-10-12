#include "sdk/wl.h"

int WifiParam_StoreOwnMac(void *);

int func_arm7_027ce03c(WlRequest *request, WlReply *reply)
{
    reply->status = 1;
    if (WIFI_DRIVER_STATE->state.mode != 16) {
        return 1;
    }
    return WifiParam_StoreOwnMac(&request->unk_10);
}
