#include "types.h"
#include "game/menu_button.h"
#include "game/menu_state.h"

extern MenuState gMenuState;
extern MenuButtonTemplate data_0215d3a8;

/* One button per CO shown on the player info screen: id = player * 2 + front
 * or back CO, 40 pixels apart per player and 16 per CO. Selects the one for
 * the player and slot the screen opened on. The screen name is a guess. */
void PlayerInfo_AddCoButtons(void) {
  int player;
  int slot;

  for (player = 0; player < gMenuState.unk_0206d67c.playerCount; player++) {
    for (slot = 0; slot < 2; slot++) {
      if (gMenuState.unk_0206d67c.coShown[player][slot] != 0) {
        MenuButton_Add(24 + player * 40 + slot * 16, 8, &data_0215d3a8,
                       player * 2 + slot);
      }
    }
  }
  MenuButton_Select(gMenuState.unk_0206d67c.slot +
                    (gMenuState.unk_0206d67c.player - 1) * 2);
}
