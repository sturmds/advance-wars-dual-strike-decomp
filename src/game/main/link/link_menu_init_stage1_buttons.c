#include "types.h"
#include "game/menu_button.h"

extern const MenuButtonTemplate data_02160178;
extern const MenuButtonTemplate data_02160128;

int func_0207a260(void);
int LinkMenu_PlayerHasMap(int index);

/* First build of the stage 1 buttons, on the side where func_0207a260() is 0: the back button
 * (254), then the buttons LinkMenu_PlayerHasMap allows, selecting the lowest id. */
void LinkMenu_InitStage1Buttons(void) {
  int i;
  int first = 255;

  if (func_0207a260() != 0) {
    return;
  }
  MenuButton_Add(0, 0, &data_02160178, 254);
  for (i = 0; i < 2; i++) {
    if (LinkMenu_PlayerHasMap(i) != 0) {
      MenuButton_Add(40, i * 16 + 56, &data_02160128, i);
      if (first > i) {
        first = i;
      }
    }
  }
  MenuButton_Select(first);
}
