#include "types.h"
#include "game/menu_state.h"

int Sound_PlaySe(int);
int func_020b9864(int);
void func_0203fbc4(void);
void func_02069b14(void);

/* Back button, sound 0x10009. Setup state 0 starts a task; any other state
 * moves to 4, and state 2 calls func_0203fbc4 first. Guessed name. */
void VsSetup_Cancel(void) {
  func_020b9864(Sound_PlaySe(0x10009));
  if (gMenuState.setup.unk_2ea != 0) {
    if (gMenuState.setup.unk_2ea == 2) {
      func_0203fbc4();
    }
    gMenuState.setup.unk_2ea = 4;
  } else {
    func_02069b14();
  }
}
