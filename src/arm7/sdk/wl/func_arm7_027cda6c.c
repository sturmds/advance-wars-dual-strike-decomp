#include "sdk/wl.h"

s32 func_arm7_03806d4c(s32 dividend, s32 divisor);
int func_arm7_027c91c0(u32 length, void *data);

int func_arm7_027cda6c(WlRequest *request, WlReply *reply)
{
    u16 length;

    reply->status = 1;
    if (WIFI_DRIVER_STATE->state.phase != 1) {
        return 11;
    }
    length = request->unk_10;
    // Guess: unk_0e counts the request halfwords after the header, so it
    // must cover the length halfword plus the data.
    if (request->unk_0e < func_arm7_03806d4c(length + 1, 2) + 1) {
        return 4;
    }
    return func_arm7_027c91c0(length, &request->unk_12);
}
