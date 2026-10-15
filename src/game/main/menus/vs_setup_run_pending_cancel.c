#include "types.h"
#include "game/menu_state.h"

void func_020b9864(void);
void func_02069b14(void);

/* Script step of the Vs setup tasks: when VsSetup_Cancel has set state 4
 * (setup.unk_2ea), runs the cancel task. */
void VsSetup_RunPendingCancel(void) {
  if (gMenuState.setup.unk_2ea != 4) {
    return;
  }
  func_020b9864();
  func_02069b14();
}
