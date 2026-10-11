#include "types.h"
#include "sdk/wm.h"

#define ALLOWED_CHANNEL_BITMAP (*(u16 *)0x027ffcfa)
#define WIRELESS_NO_CHANNEL_INFO 0x8000

/* Bit n set: channel n may be used in this region. The firmware keeps the
 * bitmap in shared memory. WIRELESS_NO_CHANNEL_INFO when the WM is not ready. */
u16 WM_GetAllowedChannel(void)
{
    if (func_02116df4() != 0) {
        return WIRELESS_NO_CHANNEL_INFO;
    }
    return ALLOWED_CHANNEL_BITMAP;
}
