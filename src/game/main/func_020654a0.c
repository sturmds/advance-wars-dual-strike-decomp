#include "types.h"
#include "game/event_queue.h"

int Link_IsActive(void);
void VsSetup_CancelSettingsMenu(void);

void func_020654a0(int unused) {
  if (Link_IsActive() == 1) {
    EventQueue_Push(33, 0);
  } else {
    VsSetup_CancelSettingsMenu();
  }
}
