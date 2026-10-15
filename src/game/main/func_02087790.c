#include "types.h"
#include "game/menu_button.h"

void Sound_PlaySe(u32 soundId);
void func_020b9864(void);

void func_02087790(void) {
  Sound_PlaySe(0x10009);
  MenuButtons_CloseAll();
  func_020b9864();
}
