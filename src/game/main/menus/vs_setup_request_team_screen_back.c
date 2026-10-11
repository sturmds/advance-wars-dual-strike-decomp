#include "types.h"
#include "game/event_queue.h"

int func_0207bd6c(void);
int func_0207bd80(void);
void *func_0206664c(void);

static inline BOOL IsFirstLinkPlayer(void)
{
    return func_0207bd6c() == 0;
}

/* Back on the team screen. In link play only the first link player acts: it
 * sends EVENT_VS_TEAM_BACK. Without link it calls func_0206664c, the same
 * call VsSetup_UpdateTeamScreenLink makes when a slot cancels. */
void VsSetup_RequestTeamScreenBack(void)
{
    if (func_0207bd80() == 1) {
        if (IsFirstLinkPlayer() != 1) {
            return;
        }
        EventQueue_Push(EVENT_VS_TEAM_BACK, 0);
    } else {
        func_0206664c();
    }
}
