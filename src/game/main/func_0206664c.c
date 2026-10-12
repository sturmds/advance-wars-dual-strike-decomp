#include "types.h"

void Sound_PlaySe(u32 soundId);
void func_020b9864(void);
void* VsSetup_OpenCoSelect(void);

void* func_0206664c(void) {
  Sound_PlaySe(0x10009);
  func_020b9864();
  return VsSetup_OpenCoSelect();
}
