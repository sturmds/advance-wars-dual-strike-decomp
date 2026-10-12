#include "types.h"

void Sound_PlaySe(u32 soundId);
void CoSelectScreen_BeginExit(void);
void func_020b9864(void);

void func_020428f4(void) {
  Sound_PlaySe(0x10009);
  CoSelectScreen_BeginExit();
  func_020b9864();
}
