#include "types.h"

void Sound_PlaySe(u32 soundId);
void Mode_SetVariant(int second);

void func_02046e18(u8* button) {
  Sound_PlaySe(0x10002);
  Mode_SetVariant(*button);
}
