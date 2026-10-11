#include "types.h"
#include "game/Unk_0215fe84.h"
#include "game/wireless_event.h"

extern u32 data_0229c880;

void Wireless_OnRescan(WirelessEvent *event);
int func_02117df0(void (*callback)(WirelessEvent *event));
int func_02079180(void);

/* Clears Unk_0215fe84.unk_30 and starts a request that reports to
 * Wireless_OnRescan. When it does not start, goes back to state 3 and flags the
 * error through func_02079180. */
BOOL Wireless_StartRescan(void)
{
    data_0215fe84->unk_30 = 0;
    if (func_02117df0(Wireless_OnRescan) == 2) {
        return TRUE;
    }
    data_0229c880 = 3;
    func_02079180();
    return FALSE;
}
