#include "types.h"
#include "game/keypad.h"
#include "game/menu_button.h"
#include "game/menu_state.h"
#include "game/Unk_0229fb30.h"
#include "game/Unk_02160ac0.h"

void CombatSetup_SendBack(void);
void CombatSetup_SendReady(void);

/* Needed to match: the cast keeps the field offset out of the literal pool. */
#define MENU ((MenuState*)&gMenuState)

/* Shop buttons of the Combat setup screen, one per unit on sale. */
#define BUTTON_FIRST_UNIT 22
#define BUTTON_UNIT_COUNT 4

/* onKeys of the shop buttons. While the local row is not marked ready, up
 * and down move through buttons 22-25 (wrapping), A confirms the button and
 * Start asks to mark the row ready if it has units. B sends the back event
 * when the row is empty and calls Menu_CancelSelected otherwise. */
void CombatSetup_ShopOnKeys(MenuButton* button) {
  int next;
  u8 id;
  int row;
  u32 keys;

  id = button->id;
  row = MENU->combat.players[data_0229fb30.unk_45a].row;
  next = id;
  if (MENU->combat.rows[row - 1].locked == 0) {
    keys = gKeypad->repeat;
    if (keys & PAD_KEY_UP) {
      next = id - 1;
      if (next < BUTTON_FIRST_UNIT) {
        next = BUTTON_FIRST_UNIT + BUTTON_UNIT_COUNT - 1;
      }
    }
    if (keys & PAD_KEY_DOWN) {
      next++;
      if (next >= BUTTON_FIRST_UNIT + BUTTON_UNIT_COUNT) {
        next = BUTTON_FIRST_UNIT;
      }
    }
    if (next != id) {
      button = MenuButton_Select(next);
    }
    keys = gKeypad->pressed;
    if (keys & PAD_BUTTON_A) {
      MenuButton_Confirm(button);
      return;
    }
    if (keys & PAD_BUTTON_START) {
      if (LINK_ROWS[row - 1].spawnCount != 0) {
        CombatSetup_SendReady();
      }
      return;
    }
  }
  if (gKeypad->pressed & PAD_BUTTON_B) {
    if (LINK_ROWS[row - 1].spawnCount == 0) {
      CombatSetup_SendBack();
    } else {
      Menu_CancelSelected();
    }
  }
}
