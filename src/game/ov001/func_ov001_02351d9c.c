#include "types.h"

void Sound_PlaySubSe(u32 soundId);
void ScreenShake_Start(int script, int arg, int parent);

void func_ov001_02351d9c(void) {
  Sound_PlaySubSe(164);
  ScreenShake_Start(3, 0, 0);
}
