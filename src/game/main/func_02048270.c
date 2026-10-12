#include "types.h"

void Sound_PlaySe(u32 soundId);
void Menu_Dismiss(void);

void func_02048270(void) {
  Sound_PlaySe(0x10006);
  Menu_Dismiss();
}
