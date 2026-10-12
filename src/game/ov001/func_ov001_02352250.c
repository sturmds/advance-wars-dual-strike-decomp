#include "types.h"

BOOL Campaign_IsHardMode(void);
void Sound_PlaySe(u32 soundId);

void func_ov001_02352250(void) {
  if (Campaign_IsHardMode()) {
    return;
  }
  Sound_PlaySe(155);
}
