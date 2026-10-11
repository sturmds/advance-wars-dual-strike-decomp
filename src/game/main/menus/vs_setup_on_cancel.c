#include "types.h"
#include "game/event_queue.h"

int Link_GetOwnPlayer(void);
int Link_IsActive(void);
void func_02069460(int unused);

static inline BOOL IsFirstLinkPlayer(void)
{
    return Link_GetOwnPlayer() == 0;
}

/* Cancel callback of the player and CO buttons of the Vs setup screen. In
 * link play only the first link player acts: it sends EVENT_VS_SETUP_CANCEL. Without
 * link it calls func_02069460, which does the same. */
void VsSetup_OnCancel(void)
{
    if (Link_IsActive() == 1) {
        if (IsFirstLinkPlayer() != 1) {
            return;
        }
        EventQueue_Push(EVENT_VS_SETUP_CANCEL, 0);
    } else {
        func_02069460(0);
    }
}
