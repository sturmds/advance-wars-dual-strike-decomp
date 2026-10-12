#include "types.h"

void Sound_PlaySe(u32 soundId);
void Script_StartWithHook(void);

void func_020d1420(void) {
  Sound_PlaySe(69);
  Script_StartWithHook();
}
