#include "types.h"

void Sound_PlaySe(u32 soundId);
void CoSelectScreen_BeginExit(void);
void func_020b9890(void);

void func_02041aac(void) {
  Sound_PlaySe(0x10006);
  CoSelectScreen_BeginExit();
  func_020b9890();
}
