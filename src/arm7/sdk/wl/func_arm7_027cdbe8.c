#include "sdk/wl.h"

int func_arm7_027c9c84(int);

int func_arm7_027cdbe8(WlRequest *request, WlReply *reply)
{
    reply->status = 1;
    if (WIFI_DRIVER_STATE->state.mode < 16) {
        return 1;
    }
    return func_arm7_027c9c84(request->unk_10);
}
