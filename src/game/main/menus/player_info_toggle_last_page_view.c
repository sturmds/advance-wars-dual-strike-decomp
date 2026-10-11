#include "types.h"
#include "game/menu_button.h"
#include "game/menu_state.h"

extern MenuState gMenuState;

void Sound_PlaySe(u32 soundId);

/* R on the last page (3): switches its view between 0 and 1. Not matching
 * yet (97%): the original keeps the state address in r1 and the value in r0
 * for the toggle, ours swaps them. */
void PlayerInfo_ToggleLastPageView(MenuButton *button)
{
    if (gMenuState.unk_0206d67c.page != 3) {
        return;
    }
    Sound_PlaySe(0x10006);
    gMenuState.unk_0206d67c.unk_0a4++;
    gMenuState.unk_0206d67c.unk_0a4 &= 1;
}
