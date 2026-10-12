#include "types.h"
#include "game/event_queue.h"

int Link_IsActive(void);
void VsSetup_Cancel(void);

void func_02069460(int unused) {
  if (Link_IsActive() == 1) {
    EventQueue_Push(18, 0);
  } else {
    VsSetup_Cancel();
  }
}
