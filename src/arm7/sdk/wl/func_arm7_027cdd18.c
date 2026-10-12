#include "sdk/wl.h"

int func_arm7_027c933c(u32 value);

// Index value that selects every entry from 1 up.
#define WIFI_EVERY_STATION_INDEX 0xffff

int func_arm7_027cdd18(WlRequest *request, WlReply *reply)
{
    // WlLimits.unk_f0 is declared u32; it holds the entry table pointer.
    Unk_027cdd18 *entries = (Unk_027cdd18 *)WIFI_DRIVER_STATE->limits.unk_f0;
    u32 i;

    reply->status = 1;
    if (request->unk_10 >= WIFI_DRIVER_STATE->limits.unk_f6 && request->unk_10 != WIFI_EVERY_STATION_INDEX) {
        return 5;
    }
    if (request->unk_14 > 63 && request->unk_14 != 0xffff) {
        return 5;
    }
    if (request->unk_10 == WIFI_EVERY_STATION_INDEX) {
        for (i = 1; i < WIFI_DRIVER_STATE->limits.unk_f6; i++) {
            entries[i].unk_1a = request->unk_12;
            if (entries[i].unk_18 != 0) {
                entries[i].unk_18 = request->unk_12;
            }
        }
    } else if (request->unk_10 != 0) {
        entries[request->unk_10].unk_1a = request->unk_12;
        if (entries[request->unk_10].unk_18 != 0) {
            entries[request->unk_10].unk_18 = request->unk_12;
        }
    }
    if (request->unk_14 != 0) {
        func_arm7_027c933c(request->unk_14);
    }
    return 0;
}
