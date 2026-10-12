#include "types.h"

/* Sets byte 4 of every active entry of gMenuButtons to at least 3. */
void MenuButtons_CloseAll(void);
void func_020b9890(void);

void func_020858bc(void) {
  MenuButtons_CloseAll();
  func_020b9890();
}
