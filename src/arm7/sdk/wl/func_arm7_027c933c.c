#include "sdk/wl.h"

int func_arm7_027c933c(u32 value)
{
    WlWork *work = WIFI_DRIVER_STATE;
    WlLink *link = WIFI_LINK_VIEW_OF(work);
    WlHead *head = WIFI_SESSION_HEAD_OF(work);
    u32 scaled;

    if (value == 0xffff) {
        head->unk_1c = 0xff;
        link->unk_8c = 0xffff;
    } else {
        scaled = value * link->beaconInterval / 100;
        if (scaled > 0x10000) {
            return 5;
        }
        head->unk_1c = value;
        link->unk_8c = scaled;
    }
    return 0;
}
