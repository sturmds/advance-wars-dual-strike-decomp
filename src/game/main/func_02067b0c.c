#include "types.h"

void Sound_PlaySe(u32 soundId);
void func_020b9890(void);
void func_02069b14(void);

void func_02067b0c(void) {
  Sound_PlaySe(0x10006);
  func_020b9890();
  func_02069b14();
}
