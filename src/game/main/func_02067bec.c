#include "types.h"
#include "game/menu_state.h"

int Sound_PlaySe(int);
int Link_FindSlotOfPlayer(int player);
int func_0206b608(int player);
void func_0206b748(int slot, u16 x, int y);
int func_0207bd6c(void);
void func_020681c8(int slot);
extern MenuState gMenuState;

void func_02067bec(int slot)
{
    MenuState *shifted = (MenuState *)((u8 *)&gMenuState + slot);

    if (shifted->setup.unk_2d0[0] != 0) {
        return;
    }
    Sound_PlaySe(0x10006);
    shifted->setup.unk_2d0[0] = 1;
    func_0206b748(slot, func_0206b608(slot) * 8 + 8, 96);
    if (slot != Link_FindSlotOfPlayer(func_0207bd6c()) - 1) {
        return;
    }
    func_020681c8(slot);
}
