#include "types.h"
#include "game/menu_button.h"

/* Sets byte 4 of every active entry of gMenuButtons to at least 3. */
void func_020b9890(void);

void func_020858bc(void) {
  MenuButtons_CloseAll();
  func_020b9890();
}
