#include "sdk/wl.h"

int WifiParam_StoreRetryLimit(u16 value)
{
    if (value > 255) {
        return 5;
    }
    WIFI_DRIVER_STATE->unk_2fe = value;
    WIFI_MAC_RETRY_LIMIT = value;
    return 0;
}
