#include "types.h"

void Sound_PlaySe(u32 soundId);
void SurvivalSelect_Leave(void);

void func_02045a9c(void) {
  Sound_PlaySe(0x10009);
  SurvivalSelect_Leave();
}
