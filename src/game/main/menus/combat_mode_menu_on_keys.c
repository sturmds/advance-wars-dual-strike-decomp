#include "types.h"
#include "game/keypad.h"
#include "game/menu_button.h"

int CombatModeMenu_SlotBelow(int slot);
int CombatModeMenu_SlotAbove(int slot);
int CombatModeMenu_SlotRight(int slot);
int CombatModeMenu_SlotLeft(int slot);

extern u8* data_0229e144;

/* Key handler of the Combat mode menu buttons. Button ids are slot numbers
 * (3 per row). The d-pad moves to the next available slot; up and down also
 * store the new slot in data_0229e144 as the row that left and right come
 * back to. A presses the selected button and B cancels. */
void CombatModeMenu_OnKeys(MenuButton* button) {
  int slot = button->id;

  if (gKeypad->repeat & PAD_KEY_LEFT) {
    slot = CombatModeMenu_SlotLeft(slot);
  }
  if (gKeypad->repeat & PAD_KEY_RIGHT) {
    slot = CombatModeMenu_SlotRight(slot);
  }
  if (gKeypad->repeat & PAD_KEY_UP) {
    slot = CombatModeMenu_SlotAbove(slot);
    *data_0229e144 = slot;
  }
  if (gKeypad->repeat & PAD_KEY_DOWN) {
    slot = CombatModeMenu_SlotBelow(slot);
    *data_0229e144 = slot;
  }
  if (slot != button->id) {
    button = MenuButton_Select(slot);
  }
  if (gKeypad->pressed & PAD_BUTTON_A) {
    MenuButton_Confirm(button);
  } else if (gKeypad->pressed & PAD_BUTTON_B) {
    Menu_CancelSelected();
  }
}
