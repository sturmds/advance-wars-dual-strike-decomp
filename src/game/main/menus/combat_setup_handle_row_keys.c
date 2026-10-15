#include "types.h"
#include "game/keypad.h"
#include "game/menu_state.h"
#include "game/Unk_0229fb30.h"

void func_02085d4c(void);
void CombatSetup_PushMarkReady(void);
void func_02085d74(void);
void func_02085d88(void);
void func_02085d9c(void);
void func_02085db0(void);

/* Needed to match: the cast keeps the field offset out of the literal pool. */
#define MENU ((MenuState*)&gMenuState)

/* Keys of the team phase of the Combat setup screen, while the local
 * player is not ready. Left and right move the player earlier and later in
 * their row (messages 14 and 15), up and down to the previous and next row
 * (12 and 13), A marks the player ready (9). B marks the player not ready
 * (10); it is also sent while ready. */
void CombatSetup_HandleRowKeys(void) {
  if (MENU->combat.players[data_0229fb30.unk_45a].ready == 0) {
    if (gKeypad->repeat & PAD_KEY_LEFT) {
      func_02085d88();
    }
    if (gKeypad->repeat & PAD_KEY_RIGHT) {
      func_02085d74();
    }
    if (gKeypad->repeat & PAD_KEY_UP) {
      func_02085db0();
    }
    if (gKeypad->repeat & PAD_KEY_DOWN) {
      func_02085d9c();
    }
    if (gKeypad->pressed & PAD_BUTTON_A) {
      CombatSetup_PushMarkReady();
      return;
    }
  }
  if (gKeypad->pressed & PAD_BUTTON_B) {
    func_02085d4c();
  }
}
