#include "types.h"

void MenuButtons_CloseAll(void);
void Sound_PlaySe(u32 soundId);
void func_02072a28(void);
void func_020b9864(void);

/* The host backs out of the link screen: closes the buttons and ends the screen with a sound effect. */
void LinkScreen_Cancel(void) {
  MenuButtons_CloseAll();
  func_02072a28();
  Sound_PlaySe(0x10009);
  func_020b9864();
}
