#include "types.h"
#include "game/keypad.h"
#include "game/menu_button.h"
#include "game/Unk_02160ac0.h"

void func_02089260(void);
void CombatModeMenu_FinishShop(void);

#define BUTTON_FIRST_UNIT 8
#define BUTTON_LAST_UNIT 11

/* Key handler of the Combat mode shop entries (buttons 8-11). Up and down
 * wrap around the four entries. A buys, B leaves the shop (or takes back the
 * last unit when one was bought) and START finishes once a unit is bought. */
void CombatModeMenu_ShopOnKeys(MenuButton* button) {
  int next = button->id;

  if (gKeypad->repeat & PAD_KEY_UP) {
    next--;
    if (next < BUTTON_FIRST_UNIT) {
      next = BUTTON_LAST_UNIT;
    }
  }
  if (gKeypad->repeat & PAD_KEY_DOWN) {
    next++;
    if (next >= BUTTON_LAST_UNIT + 1) {
      next = BUTTON_FIRST_UNIT;
    }
  }
  if (next != button->id) {
    button = MenuButton_Select(next);
  }
  if (gKeypad->pressed & PAD_BUTTON_A) {
    MenuButton_Confirm(button);
  } else if (gKeypad->pressed & PAD_BUTTON_B) {
    if (data_02160ac0->cellCount == 0) {
      func_02089260();
    } else {
      Menu_CancelSelected();
    }
  } else if (gKeypad->pressed & PAD_BUTTON_START) {
    if (data_02160ac0->cellCount != 0) {
      CombatModeMenu_FinishShop();
    }
  }
}
