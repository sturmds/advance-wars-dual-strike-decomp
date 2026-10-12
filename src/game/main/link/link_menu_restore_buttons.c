#include "types.h"
#include "game/menu_button.h"
#include "game/Unk_0229fb30.h"

extern MenuButtonTemplate data_02160c50;
extern MenuButtonTemplate data_02160ca0;

/* Undoes LinkMenu_ApplyCancel and LinkMenu_ApplyConfirm on player 0's console: turns the touch of the
 * entry buttons back on and adds buttons 254 and 252 again. */
void LinkMenu_RestoreButtons(void) {
  int i;

  if (data_0229fb30.unk_45a != 0) {
    return;
  }
  for (i = 0; i < 5; i++) {
    MenuButton_EnableTouch(i + 4);
    MenuButton_EnableTouch(i + 8);
    MenuButton_EnableTouch(i + 12);
  }
  MenuButton_Add(0, 0, &data_02160c50, 254);
  MenuButton_Add(0, 0, &data_02160ca0, 252);
}
