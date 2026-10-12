#include "sdk/wl.h"

int func_arm7_027c9cc4(int, int);

int func_arm7_027cdc30(WlRequest *request, WlReply *reply)
{
    reply->status = 1;
    if (WIFI_DRIVER_STATE->state.mode < 16) {
        return 1;
    }
    return func_arm7_027c9cc4(request->unk_10, request->unk_12);
}
