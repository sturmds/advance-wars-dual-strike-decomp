#include "types.h"
#include "game/menu_button.h"
#include "game/menu_state.h"
#include "game/Unk_0229fb30.h"

void Sound_PlaySe(u32 soundId);

extern MenuState gMenuState;
extern const MenuButtonTemplate data_02160d40;

/* Needed to match: the cast keeps the field offset out of the literal pool. */
#define MENU ((MenuState*)&gMenuState)

/* data_0229fb30, which sits at gMenuState + 0x1000. Needed to match: read through
 * gMenuState, so the compiler keeps the load after the store to MENU. */
#define LINK_STATE ((Unk_0229fb30*)((u8*)&gMenuState + 0x1000))

/* Link message 10: a player is no longer ready. For the local player, the
 * Ready button (254) comes back and, unless they are player 0, the Unready
 * button (252) closes. */
void CombatSetup_SetPlayerUnready(u8 player) {
  Sound_PlaySe(0x10009);
  MENU->combat.players[player].ready = 0;
  if (player != LINK_STATE->unk_45a) {
    return;
  }
  MenuButton_Add(0, 0, &data_02160d40, 254);
  if (player == 0) {
    return;
  }
  MenuButton_Close(252);
}
