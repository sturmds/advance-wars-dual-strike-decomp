#include "types.h"
#include "game/menu_button.h"
#include "game/menu_state.h"

extern MenuState gMenuState;
extern const MenuButtonTemplate data_0215f85c;

void Sound_PlaySe(u32 soundId);
int func_0207bd6c(void);

/* Sets a player's ready flag once. For the player of this console (func_0207bd6c) it also
 * closes button 254 and, for a player other than 0, adds button 253. */
void LinkScreen_SetReady(int player)
{
    if (LINK_READY_FLAG(gMenuState, player) != 0) {
        return;
    }
    Sound_PlaySe(0x10006);
    LINK_READY_FLAG(gMenuState, player) = 1;
    if (player != func_0207bd6c()) {
        return;
    }
    MenuButton_Close(254);
    if (player == 0) {
        return;
    }
    MenuButton_Add(0, 0, &data_0215f85c, 253);
}
