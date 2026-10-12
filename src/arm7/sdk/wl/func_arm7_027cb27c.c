#include "sdk/wl.h"

void func_arm7_027cb27c(u32 index)
{
    WIFI_PEER_ARRAY[index].timeout = WIFI_PEER_ARRAY[index].timeoutReload;
}
