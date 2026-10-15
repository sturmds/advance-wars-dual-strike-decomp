#include "types.h"
#include "game/Unk_ov006_0235f690.h"
#include "game/menu_button.h"

void func_ov006_02355610(void) {
  MenuButton_Remove(96);
  MenuButton_Remove(97);
  MenuButton_Remove(98);
  MenuButton_Remove(99);
  if (data_ov006_0236d640->unk_02 & 0x100) {
    MenuButton_Remove(100);
  }
}
