#include "types.h"
#include "game/menu_button.h"

MenuButton* SecondScreenTask_IsRunning(int id);
void func_ov000_022b04ec(MenuButton* button);

void func_ov000_022b0500(int id) {
  MenuButton* button = SecondScreenTask_IsRunning(id);

  if (button == NULL) {
    return;
  }
  func_ov000_022b04ec(button);
}
