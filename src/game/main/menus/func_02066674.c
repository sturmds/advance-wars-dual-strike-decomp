#include "types.h"
#include "game/menu_state.h"

void Sound_PlaySe(int id);
int Link_FindSlotOfPlayer(int player);
int func_0207bd6c(void);
void func_0206b6c4(int player);
void func_020668a8(int player);

extern MenuState gMenuState;

/* Needed to match: the cast keeps the field offset out of the literal pool. */
#define MENU ((MenuState *)&gMenuState)

void func_02066674(int player)
{
    Sound_PlaySe(0x10009);
    MENU->setup.unk_2d0[player] = 0;
    func_0206b6c4(player);
    if (player == Link_FindSlotOfPlayer(func_0207bd6c()) - 1) {
        func_020668a8(player);
    }
}
