#include "types.h"
#include "game/menu_button.h"

void VsSetup_AddPlayerButtons(void);

void func_020678e0(void) {
  MenuButtons_EnableTouch();
  VsSetup_AddPlayerButtons();
}
