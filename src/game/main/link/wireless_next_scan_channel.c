#include "types.h"

u16 WM_GetAllowedChannel(void);

/* Sets *channel to the next allowed channel after the current one, wrapping
 * after 16. False when no channel is allowed. */
BOOL Wireless_NextScanChannel(u16 *channel)
{
    u16 allowed;
    u16 candidate;
    int i;
    u16 current;

    allowed = WM_GetAllowedChannel();
    if (allowed == 0) {
        return FALSE;
    }
    current = *channel;
    i = 0;
    candidate = current;
    while (i < 16) {
        if (allowed & (1 << (candidate - 1))) {
            if (current != candidate) {
                *channel = candidate;
                return TRUE;
            }
        }
        i++;
        candidate = (candidate == 16) ? 1 : candidate + 1;
    }
    return TRUE;
}
