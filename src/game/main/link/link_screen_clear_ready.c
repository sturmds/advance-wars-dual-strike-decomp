#include "types.h"
#include "game/menu_button.h"
#include "game/menu_state.h"

extern MenuState gMenuState;
extern const MenuButtonTemplate data_0215f834;

void Sound_PlaySe(u32 soundId);
int Link_GetOwnPlayer(void);

/* Clears a player's ready flag. For the player of this console (Link_GetOwnPlayer) it also adds
 * button 254 again and, for a player other than 0, closes button 253. */
void LinkScreen_ClearReady(int player) {
  Sound_PlaySe(0x10009);
  LINK_READY_FLAG(gMenuState, player) = 0;
  if (player != Link_GetOwnPlayer()) {
    return;
  }
  MenuButton_Add(0, 0, &data_0215f834, 254);
  if (player != 0) {
    MenuButton_Close(253);
  }
}
