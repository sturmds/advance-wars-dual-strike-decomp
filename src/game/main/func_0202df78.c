#include "types.h"

void Sound_PlaySe(u32 soundId);
void MainMenu_ShowHelpText(void);

void func_0202df78(void) {
  Sound_PlaySe(0x10002);
  MainMenu_ShowHelpText();
}
