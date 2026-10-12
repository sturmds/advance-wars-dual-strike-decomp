#include "types.h"

void Sound_PlaySe(u32 soundId);
void SurvivalSelect_Leave(void);

void func_020459cc(void) {
  Sound_PlaySe(0x10009);
  SurvivalSelect_Leave();
}
