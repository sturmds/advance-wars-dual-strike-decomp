#include "types.h"
#include "game/event_queue.h"

int Link_IsActive(void);
void VsSetup_ConfirmTeams(void);

void VsSetup_RequestConfirmTeams(int unused) {
  if (Link_IsActive() == 1) {
    EventQueue_Push(27, 0);
  } else {
    VsSetup_ConfirmTeams();
  }
}
