#include "sdk/wl.h"

BOOL func_arm7_027caab4(WlTxSlot *slot);

// Releases the first MP reply slot (TX slots 4 and 5) whose frame has
// been sent; returns TRUE if one was released.
BOOL WifiMp_FreeSentReplySlot(void)
{
    WlTxSlots *slots = WIFI_TX_LOCATIONS_OF(WIFI_DRIVER_STATE);

    if (func_arm7_027caab4(&slots->slot[WIFI_TXLOC_POLL_ANSWER_A])) {
        return TRUE;
    }
    if (func_arm7_027caab4(&slots->slot[WIFI_TXLOC_POLL_ANSWER_B])) {
        return TRUE;
    }
    return FALSE;
}
