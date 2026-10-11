#include "types.h"
#include "game/menu_button.h"
#include "game/menu_state.h"
#include "game/Unk_0229fb30.h"

void Sound_PlaySe(u32 soundId);

extern MenuState gMenuState;
extern const MenuButtonTemplate data_02160de0;

/* Needed to match: the cast keeps the field offset out of the literal pool. */
#define MENU ((MenuState *)&gMenuState)

/* Link message 9: a player is ready. A player who is already ready is
 * skipped. For the local player, the Ready button (254) closes and, unless
 * they are player 0, the Unready button (252) is added. */
void func_02085974(u8 player)
{
    if (MENU->combat.players[player].unk_1 != 0) {
        return;
    }
    Sound_PlaySe(0x10006);
    MENU->combat.players[player].unk_1 = 1;
    if (player != data_0229fb30.unk_45a) {
        return;
    }
    MenuButton_Close(254);
    if (player == 0) {
        return;
    }
    MenuButton_Add(0, 0, &data_02160de0, 252);
}
