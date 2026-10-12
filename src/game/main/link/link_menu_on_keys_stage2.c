#include "types.h"
#include "game/keypad.h"
#include "game/menu_button.h"

void func_0207e484(MenuButton* button);

/* Key handler of the stage 2 buttons: up and down move through ids 2-4 (wrapping, skipping ids
 * with no button), A queues the pick through func_0207e484. */
void LinkMenu_OnKeysStage2(MenuButton* button) {
  int id = button->id;

  if (gKeypad->repeat & PAD_KEY_UP) {
    do {
      if (id == 2) {
        id = 4;
      } else {
        id--;
      }
    } while (MenuButton_Find(id) == NULL);
  }
  if (gKeypad->repeat & PAD_KEY_DOWN) {
    do {
      if (id == 4) {
        id = 2;
      } else {
        id++;
      }
    } while (MenuButton_Find(id) == NULL);
  }
  if (id != button->id) {
    button = MenuButton_Select(id);
  }
  if (gKeypad->pressed & PAD_BUTTON_A) {
    func_0207e484(button);
  }
}
