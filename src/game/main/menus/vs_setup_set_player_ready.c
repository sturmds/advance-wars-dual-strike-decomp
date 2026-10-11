#include "types.h"
#include "game/menu_state.h"

int Sound_PlaySe(int);
int Link_FindSlotOfPlayer(int player);
int VsSetup_GetPanelTileX(int player);
void VsSetup_StartPlayerTask(int slot, u16 x, int y);
int Link_GetOwnPlayer(void);
void VsSetup_CloseLinkButtons(int slot);
extern MenuState gMenuState;

/* Marks player slot `slot` ready on the Vs setup screen once, with a sound,
 * and starts its panel task. For this machine's own slot it also closes the
 * slot's buttons. */
void VsSetup_SetPlayerReady(int slot)
{
    MenuState *shifted = (MenuState *)((u8 *)&gMenuState + slot);

    if (shifted->setup.unk_2d0[0] != 0) {
        return;
    }
    Sound_PlaySe(0x10006);
    shifted->setup.unk_2d0[0] = 1;
    VsSetup_StartPlayerTask(slot, VsSetup_GetPanelTileX(slot) * 8 + 8, 96);
    if (slot != Link_FindSlotOfPlayer(Link_GetOwnPlayer()) - 1) {
        return;
    }
    VsSetup_CloseLinkButtons(slot);
}
