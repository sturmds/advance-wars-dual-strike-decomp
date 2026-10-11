#include "types.h"
#include "game/menu_state.h"

int Sound_PlaySe(int);
int Link_FindSlotOfPlayer(int player);
int VsSetup_GetPanelTileX(int player);
void VsSetup_StartPlayerTask(int slot, u16 x, int y);
int func_0207bd6c(void);
void VsSetup_CloseLinkButtons(int slot);
extern MenuState gMenuState;

void func_02067bec(int slot)
{
    MenuState *shifted = (MenuState *)((u8 *)&gMenuState + slot);

    if (shifted->setup.unk_2d0[0] != 0) {
        return;
    }
    Sound_PlaySe(0x10006);
    shifted->setup.unk_2d0[0] = 1;
    VsSetup_StartPlayerTask(slot, VsSetup_GetPanelTileX(slot) * 8 + 8, 96);
    if (slot != Link_FindSlotOfPlayer(func_0207bd6c()) - 1) {
        return;
    }
    VsSetup_CloseLinkButtons(slot);
}
