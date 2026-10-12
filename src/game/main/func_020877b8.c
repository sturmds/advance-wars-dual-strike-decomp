#include "types.h"

void Sound_PlaySe(u32 soundId);
void MenuButtons_CloseAll(void);
void func_020b9890(void);

void func_020877b8(void) {
  Sound_PlaySe(0x10006);
  MenuButtons_CloseAll();
  func_020b9890();
}
