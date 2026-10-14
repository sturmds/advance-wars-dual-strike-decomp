/* Needed to match: the original computes the row's address again after the
 * button calls, which mwcc only does with common subexpression removal off. */
#pragma opt_common_subs off
#include "types.h"
#include "game/menu_state.h"
#include "game/Unk_0229fb30.h"
#include "game/Unk_02160ac0.h"
#include "game/menu_button.h"

extern MenuState gMenuState;
extern MenuButtonTemplate data_02160bd8;
extern MenuButtonTemplate data_02160db8;
extern MenuButtonTemplate data_02160e58;

/* Needed to match: the cast keeps the field offset out of the literal pool. */
#define MENU ((MenuState*)&gMenuState)

/* Combat setup buttons: 22-25 one per unit on sale; 254 likely the ready
 * button (CombatSetup_MarkReady closes it when it sets the mark). */
#define BUTTON_FIRST_UNIT 22
#define BUTTON_READY 254

/* Adds the buttons of the shop to the screen of the local player, when they
 * own their row: the four unit buttons, and the ready button when the row
 * has at least rows[].memberCount units. */
void CombatSetup_AddShopButtons(void) {
  int y;
  int row; /* index into rows[]; players[].row counts from 1 */
  int id;

  y = 24;
  row = MENU->combat.players[data_0229fb30.unk_45a].row - 1;
  if (data_0229fb30.unk_45a == MENU->combat.rows[row].player) {
    for (id = BUTTON_FIRST_UNIT; id < BUTTON_FIRST_UNIT + 4; id++) {
      MenuButton_Add(8, y, &data_02160db8, id);
      y += 16;
    }
    MenuButton_EnableTouch(253);
    if (LINK_ROWS[row].spawnCount >= MENU->combat.rows[row].memberCount) {
      MenuButton_Add(0, 0, &data_02160e58, BUTTON_READY);
    }
    MenuButton_Add(0, 0, &data_02160bd8, 252);
  }
}
