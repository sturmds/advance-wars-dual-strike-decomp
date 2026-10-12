#include "types.h"
#include "game/event_queue.h"

int Link_GetOwnPlayer(void);
int Link_IsActive(void);
void* func_0206664c(void);

static inline BOOL IsFirstLinkPlayer(void) { return Link_GetOwnPlayer() == 0; }

/* Back on the team screen. In link play only the first link player acts: it
 * sends EVENT_VS_TEAM_BACK. Without link it calls func_0206664c, the same
 * call VsSetup_UpdateTeamScreenLink makes when a slot cancels. */
void VsSetup_RequestTeamScreenBack(void) {
  if (Link_IsActive() == 1) {
    if (IsFirstLinkPlayer() != 1) {
      return;
    }
    EventQueue_Push(EVENT_VS_TEAM_BACK, 0);
  } else {
    func_0206664c();
  }
}
