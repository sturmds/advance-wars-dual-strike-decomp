#include "types.h"
#include "game/menu_button.h"
#include "game/menu_state.h"
#include "game/Unk_0229fb30.h"

extern const MenuButtonTemplate data_02160c28;
extern const MenuButtonTemplate data_02160c78;

/* Needed to match: the cast keeps the field offset out of the literal pool. */
#define MENU ((MenuState*)&gMenuState)

/* If the local player is the first member of their row, turns touch back on
 * for the row buttons 20 and 21 and adds the Ready (254) and Unready (252)
 * buttons. */
void CombatSetup_AddLeaderButtons(void) {
  int player = data_0229fb30.unk_45a;

  if (player !=
      MENU->combat.rows[MENU->combat.players[player].row - 1].members[0]) {
    return;
  }
  MenuButton_EnableTouch(20);
  MenuButton_EnableTouch(21);
  MenuButton_Add(0, 0, &data_02160c28, 254);
  MenuButton_Add(0, 0, &data_02160c78, 252);
}
