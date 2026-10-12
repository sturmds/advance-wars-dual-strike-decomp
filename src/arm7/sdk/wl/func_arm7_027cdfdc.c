#include "sdk/wl.h"

int func_arm7_027ca2cc(int);

int func_arm7_027cdfdc(WlRequest *request, WlReply *reply)
{
    reply->status = 1;
    if (WIFI_DRIVER_STATE->state.mode != 16) {
        return 1;
    }
    return func_arm7_027ca2cc(request->unk_10);
}
