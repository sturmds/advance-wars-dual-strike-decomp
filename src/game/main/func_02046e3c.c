#include "types.h"

void Sound_PlaySe(u32 soundId);
void func_020b9864(void);
void func_02046f74(void);

void func_02046e3c(void) {
  Sound_PlaySe(0x10009);
  func_020b9864();
  func_02046f74();
}
