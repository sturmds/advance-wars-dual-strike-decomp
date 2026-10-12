#include "types.h"
#include "game/Unk_0229fb30.h"
#include "game/menu_button.h"

extern const MenuButtonTemplate data_02160cf0;
extern const MenuButtonTemplate data_02160e30;
extern const MenuButtonTemplate data_02160ea8;

/* Only when data_0229fb30.unk_45a is 0: adds the row of four buttons (ids 0 to 3, 40 pixels
 * apart), then the buttons 254 and 252. */
void func_02087b88(void) {
  int i;

  if (data_0229fb30.unk_45a != 0) {
    return;
  }
  for (i = 0; i < 4; i++) {
    MenuButton_Add(52 + i * 40, 80, &data_02160e30, i);
  }
  MenuButton_Add(0, 0, &data_02160ea8, 254);
  MenuButton_Add(0, 0, &data_02160cf0, 252);
}
