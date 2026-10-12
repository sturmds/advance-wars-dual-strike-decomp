#include "types.h"
#include "game/menu_button.h"

void Sound_PlaySe(u32 soundId);
void func_02070824(void);

void func_02070800(MenuButton* button) {
  Sound_PlaySe(0x10009);
  func_02070824();
}
