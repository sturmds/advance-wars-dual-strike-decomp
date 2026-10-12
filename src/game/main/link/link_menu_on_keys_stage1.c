#include "types.h"
#include "game/keypad.h"
#include "game/menu_button.h"

void func_0207e5d0(void);
void func_0207e5bc(MenuButton* button);

/* Key handler of the stage 1 buttons (ids 0 and 1): up and down switch to the other one, B queues
 * event 2 (func_0207e5d0), A queues event 1 with the button id (func_0207e5bc). */
void LinkMenu_OnKeysStage1(MenuButton* button) {
  int id = button->id;

  if (gKeypad->repeat & (PAD_KEY_UP | PAD_KEY_DOWN)) {
    do {
      if (id == 0) {
        id = 1;
      } else {
        id = 0;
      }
    } while (MenuButton_Find(id) == NULL);
  }
  if (id != button->id) {
    button = MenuButton_Select(id);
  }
  if (gKeypad->pressed & PAD_BUTTON_B) {
    func_0207e5d0();
  } else if (gKeypad->pressed & PAD_BUTTON_A) {
    func_0207e5bc(button);
  }
}
