#include "types.h"
#include "game/menu_button.h"
#include "game/menu_state.h"

extern const MenuButtonTemplate data_0215f884;
extern const MenuButtonTemplate data_0215f834;
extern const MenuButtonTemplate data_0215f85c;

int LinkScreen_GetPlayerRow(int player);
int Link_GetOwnPlayer(void);

/* One button per player (ids 0 up), then button 254, and button 253 when Link_GetOwnPlayer returns 0. */
void LinkScreen_AddButtons(void) {
  int i;

  for (i = 0; i < gMenuState.link.playerCount; i++) {
    MenuButton_Add(32, LinkScreen_GetPlayerRow(i) * 8, &data_0215f884, i);
  }
  MenuButton_Add(0, 0, &data_0215f834, 254);
  if (Link_GetOwnPlayer() != 0) {
    return;
  }
  MenuButton_Add(0, 0, &data_0215f85c, 253);
}
