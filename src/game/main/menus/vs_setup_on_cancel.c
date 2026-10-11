#include "types.h"
#include "game/event_queue.h"

int func_0207bd6c(void);
int func_0207bd80(void);
void func_02069460(int unused);

static inline BOOL IsFirstLinkPlayer(void)
{
    return func_0207bd6c() == 0;
}

/* Cancel callback of the player and CO buttons of the Vs setup screen. In
 * link play only the first link player acts: it sends EVENT_VS_SETUP_CANCEL. Without
 * link it calls func_02069460, which does the same. */
void VsSetup_OnCancel(void)
{
    if (func_0207bd80() == 1) {
        if (IsFirstLinkPlayer() != 1) {
            return;
        }
        EventQueue_Push(EVENT_VS_SETUP_CANCEL, 0);
    } else {
        func_02069460(0);
    }
}
