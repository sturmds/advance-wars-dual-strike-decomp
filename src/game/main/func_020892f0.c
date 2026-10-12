#include "types.h"

void Sound_PlaySe(u32 soundId);
void Menu_DrawSelectedHelpText(void);

void func_020892f0(void) {
  Sound_PlaySe(0x10002);
  Menu_DrawSelectedHelpText();
}
