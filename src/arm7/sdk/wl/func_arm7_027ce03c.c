#include "sdk/wl.h"

int func_arm7_027ca340(void *);

int func_arm7_027ce03c(WlRequest *request, WlReply *reply)
{
    reply->status = 1;
    if (WIFI_DRIVER_STATE->state.mode != 16) {
        return 1;
    }
    return func_arm7_027ca340(&request->unk_10);
}
