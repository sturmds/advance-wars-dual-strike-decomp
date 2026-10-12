#include "types.h"
#include "game/menu_button.h"
#include "game/menu_state.h"
#include "game/Unk_0229fb30.h"

void Sound_PlaySe(u32 soundId);

extern MenuState gMenuState;
extern const MenuButtonTemplate data_02160de0;

/* Needed to match: the cast keeps the field offset out of the literal pool. */
#define MENU ((MenuState*)&gMenuState)

/* data_0229fb30, which sits at gMenuState + 0x1000. Needed to match: read through
 * gMenuState, so the compiler keeps the load after the store to MENU. */
#define LINK_STATE ((Unk_0229fb30*)((u8*)&gMenuState + 0x1000))

/* Link message 9: a player is ready. A player who is already ready is
 * skipped. For the local player, the Ready button (254) closes and, unless
 * they are player 0, the Unready button (252) is added. */
void CombatSetup_SetPlayerReady(u8 player) {
  if (MENU->combat.players[player].ready != 0) {
    return;
  }
  Sound_PlaySe(0x10006);
  MENU->combat.players[player].ready = 1;
  if (player != LINK_STATE->unk_45a) {
    return;
  }
  MenuButton_Close(254);
  if (player == 0) {
    return;
  }
  MenuButton_Add(0, 0, &data_02160de0, 252);
}
