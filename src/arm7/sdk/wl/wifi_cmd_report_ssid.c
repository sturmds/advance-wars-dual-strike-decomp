#include "sdk/wl.h"

int WifiCmd_ReportSsid(WlRequest *request, WlReply *reply)
{
    u16 *dst;
    u32 i;
    u16 *src;

    reply->status = 18;
    reply->value = WIFI_DRIVER_STATE->state.unk_36;
    dst = reply->words;
    src = WIFI_DRIVER_STATE->state.words;
    for (i = 0; i < 32; i += 2) {
        *dst++ = *src++;
    }
    return 0;
}
