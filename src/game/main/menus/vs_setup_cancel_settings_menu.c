#include "types.h"

void Sound_PlaySe(u32 soundId);
void func_020b9864(void);
void func_020654dc(void);

void VsSetup_CancelSettingsMenu(void) {
  Sound_PlaySe(0x10009);
  func_020b9864();
  func_020654dc();
}
