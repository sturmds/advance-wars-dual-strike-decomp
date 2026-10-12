#include "types.h"
#include "game/event_queue.h"

int Link_IsActive(void);
void func_02067b0c(void);

void VsSetup_Confirm(int unused) {
  if (Link_IsActive() == 1) {
    EventQueue_Push(17, 0);
  } else {
    func_02067b0c();
  }
}
