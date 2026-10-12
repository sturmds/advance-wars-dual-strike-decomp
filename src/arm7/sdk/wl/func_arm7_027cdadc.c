#include "sdk/wl.h"

int func_arm7_027c9a20(int);

int func_arm7_027cdadc(WlRequest *request, WlReply *reply)
{
    u16 phase;

    reply->status = 1;
    phase = WIFI_DRIVER_STATE->state.phase;
    if (phase != 2 && phase != 3) {
        return 11;
    }
    return func_arm7_027c9a20(request->unk_10);
}
