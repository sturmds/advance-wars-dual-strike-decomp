#include "types.h"

void Sound_StopSubSe(int fadeFrames);
void Sound_PlaySe(u32 soundId);

void func_ov001_02351d50(void) {
  Sound_StopSubSe(20);
  Sound_PlaySe(165);
}
