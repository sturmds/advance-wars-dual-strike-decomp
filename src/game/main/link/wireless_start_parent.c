#include "types.h"
#include "game/Unk_0215fe84.h"
#include "sdk/wm.h"

void Wireless_OnParentEvent(WMStartParentCallback *cb);
void Wireless_ClearPlayerName(u8 aid);
int WM_StartParent(void (*callback)(WMStartParentCallback *cb));

/* Forgets all children, then starts the parent role with
 * Wireless_OnParentEvent as callback. True when the request started. */
BOOL Wireless_StartParent(void)
{
    int aid;

    data_0215fe84->childAids = 0;
    data_0215fe84->childCount = 0;
    for (aid = 1; aid < 8; aid++) {
        Wireless_ClearPlayerName(aid);
    }
    return WM_StartParent(Wireless_OnParentEvent) == 2;
}
