#include "types.h"
#include "game/wireless_event.h"

extern u32 data_0229c880;

void func_02078cbc(WirelessEvent *event);
int WM_EndParent(void (*callback)(WirelessEvent *event));
int func_02079180(void);

/* Parent side: starts a request that reports to func_02078cbc, which goes on
 * to Wireless_StartParent. When it does not start, goes back to state 3 and
 * calls func_02079180. The name is a guess. */
BOOL Wireless_StartParentSetup(void)
{
    if (WM_EndParent(func_02078cbc) == 2) {
        return TRUE;
    }
    data_0229c880 = 3;
    func_02079180();
    return FALSE;
}
