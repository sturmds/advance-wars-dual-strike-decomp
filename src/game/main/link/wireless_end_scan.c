#include "types.h"
#include "game/Unk_0215fe84.h"
#include "game/wireless_event.h"

void func_020788c4(WirelessEvent *event);
int func_02117888(void (*callback)(WirelessEvent *event));

/* Ends the scan when Unk_0215fe84.unk_14 is set: starts a request
 * that reports to func_020788c4 and clears the flag. True when nothing is
 * left to do or the request started. */
BOOL Wireless_EndScan(void)
{
    BOOL ok;

    if (data_0215fe84->unk_14 == 0) {
        return TRUE;
    }
    if (func_02117888(func_020788c4) != 2) {
        ok = FALSE;
    } else {
        data_0215fe84->unk_14 = 0;
        ok = TRUE;
    }
    return ok;
}
