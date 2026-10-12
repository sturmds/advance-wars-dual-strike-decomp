#include "sdk/wl.h"

int func_arm7_027ca244(int);

int func_arm7_027cdf80(WlRequest *request, WlReply *reply)
{
    reply->status = 1;
    if (WIFI_DRIVER_STATE->state.mode > 32) {
        return 1;
    }
    if (WIFI_DRIVER_STATE->state.mode == 32 && WIFI_DRIVER_STATE->state.unk_2a != 0) {
        return 1;
    }
    return func_arm7_027ca244(request->unk_10);
}
