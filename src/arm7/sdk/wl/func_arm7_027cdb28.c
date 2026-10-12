#include "sdk/wl.h"

int func_arm7_027c9a54(int);

int func_arm7_027cdb28(WlRequest *request, WlReply *reply)
{
    reply->status = 1;
    if (WIFI_DRIVER_STATE->state.phase != 1) {
        return 11;
    }
    return func_arm7_027c9a54(request->unk_10);
}
