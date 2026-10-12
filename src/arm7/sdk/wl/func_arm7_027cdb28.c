#include "sdk/wl.h"

int WifiParam_ChangeListenPeriod(int);

int func_arm7_027cdb28(WlRequest *request, WlReply *reply)
{
    reply->status = 1;
    if (WIFI_DRIVER_STATE->state.phase != 1) {
        return 11;
    }
    return WifiParam_ChangeListenPeriod(request->unk_10);
}
