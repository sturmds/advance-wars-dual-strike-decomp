#include "types.h"
#include "game/menu_button.h"
#include "game/menu_state.h"

extern MenuState gMenuState;

void Sound_PlaySe(u32 soundId);

/* Needed to match: the cast puts the state address in r1 and the value in r0. */
#define MENU ((MenuState*)&gMenuState)

/* R on the last page (3): switches its view between 0 and 1. */
void PlayerInfo_ToggleLastPageView(MenuButton* button) {
  if (MENU->unk_0206d67c.page != 3) {
    return;
  }
  Sound_PlaySe(0x10006);
  MENU->unk_0206d67c.lastPageView++;
  MENU->unk_0206d67c.lastPageView &= 1;
}
