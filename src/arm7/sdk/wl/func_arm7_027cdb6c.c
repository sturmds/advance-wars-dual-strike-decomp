#include "sdk/wl.h"

int func_arm7_027c9aa0(int);

int func_arm7_027cdb6c(WlRequest *request, WlReply *reply)
{
    reply->status = 1;
    if (WIFI_DRIVER_STATE->state.phase != 1) {
        return 11;
    }
    return func_arm7_027c9aa0(request->unk_10);
}
