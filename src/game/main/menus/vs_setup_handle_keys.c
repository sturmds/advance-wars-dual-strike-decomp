#include "types.h"
#include "game/keypad.h"
#include "game/menu_button.h"

BOOL Skills_AreDisabled(void);
void VsSetup_NextForceRankSet(int a);
void VsSetup_PrevForceRankSet(int a);
void VsSetup_ChangeSettingDown(MenuButton* button);
void VsSetup_ChangeSettingUp(MenuButton* button);
void VsSetup_OpenForceRank(int a);
void VsSetup_Confirm(int a);

/* onKeys of the Vs setup buttons (COs, rules and so on). Left and right
 * step to the previous or next button id that exists, 0-15 with wrap; up
 * and down change the selected button's setting; L and R cycle the Force
 * Rank set of the player slot in BattleSetupState.unk_2e4. */
void VsSetup_HandleKeys(MenuButton* button) {
  int cursor;

  cursor = button->id;
  if (gKeypad->repeat & PAD_KEY_LEFT) {
    do {
      if (cursor == 0) {
        cursor = 15;
      } else {
        cursor--;
      }
    } while (MenuButton_Find(cursor) == NULL);
  }
  if (gKeypad->repeat & PAD_KEY_RIGHT) {
    do {
      if (cursor == 15) {
        cursor = 0;
      } else {
        cursor++;
      }
    } while (MenuButton_Find(cursor) == NULL);
  }
  if (cursor != button->id) {
    button = MenuButton_Select(cursor);
  }
  if (gKeypad->repeat & PAD_KEY_UP) {
    VsSetup_ChangeSettingUp(button);
  }
  if (gKeypad->repeat & PAD_KEY_DOWN) {
    VsSetup_ChangeSettingDown(button);
  }
  if (gKeypad->pressed & PAD_BUTTON_L) {
    VsSetup_PrevForceRankSet(0);
  }
  if (gKeypad->pressed & PAD_BUTTON_R) {
    VsSetup_NextForceRankSet(0);
  }
  if ((gKeypad->pressed & PAD_BUTTON_X) && !Skills_AreDisabled()) {
    VsSetup_OpenForceRank(0);
    return;
  }
  if (gKeypad->pressed & PAD_BUTTON_B) {
    Menu_CancelSelected();
    return;
  }
  if (gKeypad->pressed & PAD_BUTTON_A) {
    VsSetup_Confirm(0);
  }
}
