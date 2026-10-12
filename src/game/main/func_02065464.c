#include "types.h"
#include "game/event_queue.h"

int Link_IsActive(void);
void VsSetup_CloseSettingsMenu(void);

void func_02065464(int unused) {
  if (Link_IsActive() == 1) {
    EventQueue_Push(32, 0);
  } else {
    VsSetup_CloseSettingsMenu();
  }
}
