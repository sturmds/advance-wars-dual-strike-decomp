#include "sdk/wl.h"

/*
 * Task 18: ages the BSS list and frees entries whose time ran out. The
 * original never moves on to the next entry, so only bssList[0] ages, by
 * four steps per run.
 */
void WifiTask_AgeNetworks(void)
{
    WlBssEntry *entry = WIFI_DRIVER_STATE->limits.bssList;
    u32 i;

    for (i = 0; i < WIFI_REMEMBERED_NETWORKS; i++) {
        if (entry->ttl != 0) {
            entry->ttl--;
            if (entry->ttl == 0) {
                entry->rssi = 0;
            }
        }
    }
}
