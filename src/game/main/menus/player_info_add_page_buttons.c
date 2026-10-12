#include "types.h"
#include "game/menu_button.h"
#include "game/menu_state.h"

extern MenuState gMenuState;
extern MenuButtonTemplate data_0215d308;
extern MenuButtonTemplate data_0215d330;
extern MenuButtonTemplate data_0215d358;
extern MenuButtonTemplate data_0215d380;

/* Buttons of the player info screen (PlayerInfo_DrawPage draws its pages): 254 always,
 * 253 and 252 for turning to the previous or next page, 251 on the last
 * page. The screen name is a guess. */
void PlayerInfo_AddPageButtons(void) {
  MenuButton_Add(0, 0, &data_0215d308, 254);
  if (gMenuState.unk_0206d67c.page == 3) {
    MenuButton_Add(0, 0, &data_0215d380, 251);
  }
  if (gMenuState.unk_0206d67c.page != 0) {
    MenuButton_Add(0, 0, &data_0215d330, 253);
  }
  if (gMenuState.unk_0206d67c.page < 3) {
    MenuButton_Add(0, 0, &data_0215d358, 252);
  }
}
