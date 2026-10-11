#include "types.h"
#include "game/wireless_event.h"

extern u32 data_0229c880;

void Wireless_OnEvent(WirelessEvent *event);
int func_021176b0(void (*callback)(WirelessEvent *event), int arg);
int func_02079180(void);

/* Starts a request that reports to Wireless_OnEvent. When it does not start,
 * goes back to state 3 and calls func_02079180. */
BOOL Wireless_StartEventRequest(void)
{
    if (func_021176b0(Wireless_OnEvent, 0) == 2) {
        return TRUE;
    }
    data_0229c880 = 3;
    func_02079180();
    return FALSE;
}
