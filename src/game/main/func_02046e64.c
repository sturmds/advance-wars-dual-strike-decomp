#include "types.h"

void Sound_PlaySe(u32 soundId);
void func_020b9890(void);
void func_02046f74(void);

void func_02046e64(void) {
  Sound_PlaySe(0x10006);
  func_020b9890();
  func_02046f74();
}
