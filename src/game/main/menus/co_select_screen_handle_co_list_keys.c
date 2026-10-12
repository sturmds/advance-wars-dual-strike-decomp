#include "types.h"
#include "game/co_select_screen.h"
#include "game/keypad.h"
#include "game/menu_button.h"

void CoSelectScreen_HandleR(void);
void CoSelectScreen_HandleX(void);
int CoSelectScreen_ListSlotBelow(int slot);
int CoSelectScreen_ListSlotAbove(int slot);
int CoSelectScreen_ListSlotRight(int slot);
int CoSelectScreen_ListSlotLeft(int slot);

/* onKeys of the CO list buttons. The arrows move the cursor to the next
 * slot that is empty or holds an available CO; up and down also set
 * listAnchor. A confirms, B cancels, X and R work as on the CO grid. */
void CoSelectScreen_HandleCoListKeys(MenuButton* button) {
  int slot = button->id;

  if (gKeypad->repeat & PAD_KEY_LEFT) {
    slot = CoSelectScreen_ListSlotLeft(slot);
  }
  if (gKeypad->repeat & PAD_KEY_RIGHT) {
    slot = CoSelectScreen_ListSlotRight(slot);
  }
  if (gKeypad->repeat & PAD_KEY_UP) {
    slot = CoSelectScreen_ListSlotAbove(slot);
    data_0229c73c->listAnchor = slot;
  }
  if (gKeypad->repeat & PAD_KEY_DOWN) {
    slot = CoSelectScreen_ListSlotBelow(slot);
    data_0229c73c->listAnchor = slot;
  }
  if (slot != button->id) {
    button = MenuButton_Select(slot);
  }
  if (gKeypad->pressed & PAD_BUTTON_A) {
    MenuButton_Confirm(button);
    return;
  }
  if (gKeypad->pressed & PAD_BUTTON_B) {
    Menu_CancelSelected();
    return;
  }
  if (gKeypad->pressed & PAD_BUTTON_X) {
    CoSelectScreen_HandleX();
    return;
  }
  if (gKeypad->pressed & PAD_BUTTON_R) {
    CoSelectScreen_HandleR();
  }
}
