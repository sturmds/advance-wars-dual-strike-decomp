#include "types.h"
#include "game/co_select_screen.h"
#include "game/menu_button.h"

BOOL Skills_AreDisabled(void);

extern const MenuButtonTemplate data_0215a9a0;
extern const MenuButtonTemplate data_0215a9c8;
extern const MenuButtonTemplate data_0215a9f0;
extern const MenuButtonTemplate data_0215aa68;

/* Adds the buttons of the CO select screen's grid mode (ids 0xfe, 0xfb, and
 * 0xfc unless skills are disabled). With the CO list open that is all;
 * otherwise adds button 0xfd, moved down 32 pixels when skills are disabled. */
void CoSelectScreen_AddButtons(void) {
  MenuButton_Add(0, 0, &data_0215a9a0, 0xfe);
  MenuButton_Add(0, 0, &data_0215a9f0, 0xfb);
  if (Skills_AreDisabled() == 0) {
    MenuButton_Add(0, 0, &data_0215aa68, 0xfc);
  }
  if (data_0229c73c->coListOpen != 0) {
    return;
  }
  if (Skills_AreDisabled() == 0) {
    MenuButton_Add(0, 0, &data_0215a9c8, 0xfd);
  } else {
    MenuButton_Add(0, 32, &data_0215a9c8, 0xfd);
  }
}
