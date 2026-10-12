#include "types.h"

int Wmsp_RssiToLinkLevel(u32 rssi)
{
    if (rssi < 8) {
        return 0;
    }
    if (rssi < 14) {
        return 1;
    }
    if (rssi < 20) {
        return 2;
    }
    return 3;
}
