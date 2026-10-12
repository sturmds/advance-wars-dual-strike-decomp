#include "types.h"
#include "game/keypad.h"
#include "game/menu_button.h"

int PlayerInfo_NextCoButton(int id);
int PlayerInfo_PrevCoButton(int id);
void PlayerInfo_NextPage(MenuButton* button);
void PlayerInfo_PrevPage(MenuButton* button);
void PlayerInfo_ToggleLastPageView(MenuButton* button);

/* onKeys of the CO buttons of the player info screen. Left and right move to
 * the previous or next CO, up and down turn the page, R toggles the view of
 * the last page and B cancels. */
void PlayerInfo_HandleKeys(MenuButton* button) {
  int id = button->id;

  if (gKeypad->repeat & PAD_KEY_LEFT) {
    id = PlayerInfo_PrevCoButton(id);
  }
  if (gKeypad->repeat & PAD_KEY_RIGHT) {
    id = PlayerInfo_NextCoButton(id);
  }
  if (id != button->id) {
    MenuButton_Select(id);
  }
  if (gKeypad->repeat & PAD_KEY_UP) {
    PlayerInfo_PrevPage(NULL);
  }
  if (gKeypad->repeat & PAD_KEY_DOWN) {
    PlayerInfo_NextPage(NULL);
  }
  if (gKeypad->pressed & PAD_BUTTON_R) {
    PlayerInfo_ToggleLastPageView(NULL);
  }
  if (gKeypad->pressed & PAD_BUTTON_B) {
    Menu_CancelSelected();
  }
}
